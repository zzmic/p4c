/*
Copyright 2018 VMware, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef FRONTENDS_P4_COMMONINLINING_H_
#define FRONTENDS_P4_COMMONINLINING_H_

#include "frontends/common/resolveReferences/resolveReferences.h"
#define DEBUG_INLINER 0

#if DEBUG_INLINER
#include "frontends/p4/toP4/toP4.h"
#endif
#include "frontends/p4/callGraph.h"
#include "ir/ir.h"

/**
  This file has common data structures used for inlining functions and actions.
  Inlining these is simpler than inlining controls and parsers
*/

namespace P4 {

using namespace literals;

template <class CallableT, class CallNodeT, class CallExpressionT = IR::MethodCallExpression>
class SimpleCallInfo : public IHasDbPrint {
    // Callable can be P4Action, Function, P4Control, P4Parser
 public:
    using Callable = CallableT;
    using CallNode = CallNodeT;
    using CallExpression = CallExpressionT;

    const Callable *caller;  // object that performs the call
    const Callable *callee;  // object that is called
    const CallNode *call;
    const CallExpression *callExpr;

    SimpleCallInfo(const Callable *caller, const Callable *callee, const CallNode *call,
                   const CallExpression *expr)
        : caller(caller), callee(callee), call(call), callExpr(expr) {
        CHECK_NULL(caller);
        CHECK_NULL(callee);
        CHECK_NULL(call);
        CHECK_NULL(callExpr);
    }
    void dbprint(std::ostream &out) const {
        out << dbp(callee) << " into " << dbp(caller) << " at " << dbp(call) << " via "
            << dbp(callExpr);
    }
};

template <class CallInfo>
class SimpleInlineWorkList : public IHasDbPrint {
 public:
    using ReplacementMap = std::map<
        const typename CallInfo::CallNode *,
        std::pair<const typename CallInfo::Callable *, const typename CallInfo::CallExpression *>>;
    // Map caller -> statement -> callee
    std::map<const typename CallInfo::Callable *, ReplacementMap> sites;
    void add(CallInfo *info) {
        CHECK_NULL(info);
        LOG3(info);
        sites[info->caller][info->call] = {info->callee, info->callExpr};
    }
    void dbprint(std::ostream &out) const {
        for (auto t : sites) {
            out << dbp(t.first);
            for (auto c : t.second) {
                out << std::endl
                    << "\t" << dbp(c.first) << " => " << dbp(c.second.first) << " via "
                    << dbp(c.second.second);
            }
        }
    }
    bool empty() const { return sites.empty(); }
};

template <class Callable, class CallInfo, class InlineWorkList>
class SimpleInlineList {
    std::vector<CallInfo *> toInline;     // initial data
    std::vector<CallInfo *> inlineOrder;  // sorted in inlining order

 public:
    // generate the inlining order
    void analyze() {
        // We only keep the call graph between objects of the same kind.
        P4::CallGraph<const Callable *> cg("Call-graph");
        for (auto c : toInline) cg.calls(c->caller, c->callee);

        // must inline from leaves up
        std::vector<const Callable *> order;
        cg.sort(order);
        for (auto c : order) {
            // This is quadratic, but hopefully the call graph is not too large
            for (auto ci : toInline) {
                if (ci->caller == c) inlineOrder.push_back(ci);
            }
        }

        std::reverse(inlineOrder.begin(), inlineOrder.end());
    }

    size_t size() const { return toInline.size(); }
    void clear() {
        toInline.clear();
        inlineOrder.clear();
    }

    /// Get next batch of objects to inline
    InlineWorkList *next() {
        if (inlineOrder.size() == 0) return nullptr;

        std::set<const Callable *> callers;
        auto result = new InlineWorkList();

        // Find callables that can be inlined simultaneously.
        // This traversal is in topological order starting from leaf callees.
        // We stop at the first callable which calls one of the callables
        // we have already selected.
        while (!inlineOrder.empty()) {
            auto last = inlineOrder.back();
            if (callers.find(last->callee) != callers.end()) break;
            inlineOrder.pop_back();
            result->add(last);
            callers.emplace(last->caller);
        }
        BUG_CHECK(!result->empty(), "Empty list of methods to inline");
        return result;
    }

    void add(CallInfo *aci) { toInline.push_back(aci); }

    void replace(const Callable *container, const Callable *replacement) {
        LOG2("Substituting " << container << " with " << replacement);
        for (auto e : inlineOrder) {
            if (e->callee == container) e->callee = replacement;
            if (e->caller == container) e->caller = replacement;
        }
    }
};

// Base class for inliners
template <class InlineList, class InlineWorkList>
class AbstractInliner : public Transform, public ResolutionContext {
 protected:
    InlineList *list;
    InlineWorkList *toInline;
    AbstractInliner() : list(nullptr), toInline(nullptr) {}

 public:
    void prepare(InlineList *list, InlineWorkList *toInline) {
        CHECK_NULL(list);
        CHECK_NULL(toInline);
        this->list = list;
        this->toInline = toInline;
    }
    Visitor::profile_t init_apply(const IR::Node *node) {
        LOG2("AbstractInliner " << toInline);
        return Transform::init_apply(node);
    }
    virtual ~AbstractInliner() {}
};

template <class InlineList, class InlineWorkList>
class InlineDriver : public Visitor {
    InlineList *toInline;
    AbstractInliner<InlineList, InlineWorkList> *inliner;

 public:
    InlineDriver(InlineList *toInline, AbstractInliner<InlineList, InlineWorkList> *inliner)
        : toInline(toInline), inliner(inliner) {
        CHECK_NULL(toInline);
        CHECK_NULL(inliner);
        setName(("InlineDriver_"_cs + cstring(inliner->name())).c_str());
    }
    const IR::Node *apply_visitor(const IR::Node *program, const char * = 0) override {
        LOG2("InlineDriver");
        toInline->analyze();
        LOG3("InlineList size " << toInline->size());
        while (auto todo = toInline->next()) {
            LOG2("Processing " << todo);
            inliner->prepare(toInline, todo);
            program = program->apply(*inliner);
            if (::P4::errorCount() > 0) break;

#if DEBUG_INLINER
            // debugging code; we don't have an easy way to dump the program here,
            // since we are not between passes
            ToP4 top4(&std::cout, false, nullptr);
            program->apply(top4);
#endif
        }
        return program;
    }
};

}  // namespace P4

#endif /* FRONTENDS_P4_COMMONINLINING_H_ */
