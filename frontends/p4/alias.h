/*
Copyright 2019 VMware, Inc.

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

#ifndef FRONTENDS_P4_ALIAS_H_
#define FRONTENDS_P4_ALIAS_H_

/*
 * A simple conservative syntactic alias analysis.  Two things may
 * alias if they refer to objects with the same name.  This pass may
 * be run early in the front end, before enough information is present
 * (eg.  def-use information) to do a precise alias analysis.  Also,
 * this pass is safe only if the expressions compared for aliasing are
 * part of the *same statement*.
 */

#include "frontends/common/resolveReferences/resolveReferences.h"
#include "ir/ir.h"

namespace P4 {

using namespace literals;

/// This class represents the path to a location.
/// Given a struct S { bit a; bit b; } and a variable S x;
/// a path can be x.a, or just x.  An array index is represented as a
/// number (encoded as a string) or as "*", denoting an unknown index.
struct LocationPath : public IHasDbPrint {
    const IR::IDeclaration *root;
    std::vector<cstring> path;

    explicit LocationPath(const IR::IDeclaration *root) : root(root) { CHECK_NULL(root); }

    const LocationPath *append(cstring suffix) const {
        auto result = new LocationPath(root);
        result->path = path;
        result->path.push_back(suffix);
        return result;
    }

    /// True if this path is a prefix of other or the other way around
    bool isPrefix(const LocationPath *other) const {
        // Due to the structure of the P4 language, two distinct
        // declarations can never alias.
        if (root != other->root) return false;
        size_t len = std::min(path.size(), other->path.size());
        for (size_t i = 0; i < len; i++) {
            if (path.at(i) == "*" || other->path.at(i) == "*") continue;
            if (path.at(i) != other->path.at(i)) return false;
        }
        return true;
    }

    void dbprint(std::ostream &out) const override {
        out << root->getName();
        for (auto p : path) out << "." << p;
    }
};

/// We represent a set of location set as a set of LocationPath
/// objects.
class SetOfLocations : public IHasDbPrint {
 public:
    std::set<const LocationPath *> paths;

    SetOfLocations() = default;
    explicit SetOfLocations(const LocationPath *path) { add(path); }
    explicit SetOfLocations(const SetOfLocations *set) : paths(set->paths) {}

    void add(const LocationPath *path) { paths.emplace(path); }
    bool overlaps(const SetOfLocations *other) const {
        // Normally one of these sets has only one element, because
        // one of the two is a left-value, so this should be fast.
        for (auto s : paths) {
            for (auto so : other->paths) {
                if (s->isPrefix(so)) return true;
            }
        }
        return false;
    }

    const SetOfLocations *join(const SetOfLocations *other) const {
        auto result = new SetOfLocations(this);
        for (auto p : other->paths) result->add(p);
        return result;
    }

    /// Append suffix to each location in the set
    const SetOfLocations *append(cstring suffix) const {
        auto result = new SetOfLocations();
        for (auto p : paths) {
            auto append = p->append(suffix);
            result->add(append);
        }
        return result;
    }

    void dbprint(std::ostream &out) const override {
        for (auto p : paths) out << p << std::endl;
    }
};

/// Computes the SetOfLocations read and written by an expression.
class ReadsWrites : public Inspector, public ResolutionContext {
    std::map<const IR::Expression *, const SetOfLocations *> rw;

 public:
    ReadsWrites() { setName("ReadsWrites"); }

    void postorder(const IR::Operation_Binary *expression) override {
        auto left = ::P4::get(rw, expression->left);
        auto right = ::P4::get(rw, expression->right);
        CHECK_NULL(left);
        CHECK_NULL(right);
        rw.emplace(expression, left->join(right));
    }

    void postorder(const IR::PathExpression *expression) override {
        auto decl = getDeclaration(expression->path);
        auto path = new LocationPath(decl);
        auto locs = new SetOfLocations(path);
        rw.emplace(expression, locs);
    }

    void postorder(const IR::Operation_Unary *expression) override {
        auto e = ::P4::get(rw, expression->expr);
        CHECK_NULL(e);
        rw.emplace(expression, e);
    }

    void postorder(const IR::Member *expression) override {
        auto e = ::P4::get(rw, expression->expr);
        CHECK_NULL(e);
        auto result = e->append(expression->member);
        rw.emplace(expression, result);
    }

    void postorder(const IR::ArrayIndex *expression) override {
        auto e = ::P4::get(rw, expression->left);
        CHECK_NULL(e);
        const SetOfLocations *result;
        if (expression->right->is<IR::Constant>()) {
            int index = expression->right->to<IR::Constant>()->asInt();
            result = e->append(Util::toString(index));
        } else {
            auto index = ::P4::get(rw, expression->right);
            result = e->append("*"_cs)->join(index);
        }
        rw.emplace(expression, result);
    }

    void postorder(const IR::Literal *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    void postorder(const IR::InvalidHeader *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    void postorder(const IR::InvalidHeaderUnion *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    void postorder(const IR::HeaderStackExpression *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    void postorder(const IR::TypeNameExpression *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    void postorder(const IR::Operation_Ternary *expression) override {
        auto e0 = ::P4::get(rw, expression->e0);
        auto e1 = ::P4::get(rw, expression->e1);
        auto e2 = ::P4::get(rw, expression->e2);
        CHECK_NULL(e0);
        CHECK_NULL(e1);
        CHECK_NULL(e2);
        rw.emplace(expression, e0->join(e1)->join(e2));
    }

    void postorder(const IR::AbstractSlice *expression) override {
        auto e = ::P4::get(rw, expression->e0);
        CHECK_NULL(e);
        rw.emplace(expression, e);
    }

    void postorder(const IR::MethodCallExpression *expression) override {
        auto e = ::P4::get(rw, expression->method);
        for (auto a : *expression->arguments) {
            auto s = ::P4::get(rw, a->expression);
            CHECK_NULL(s);
            e = e->join(s);
        }
        rw.emplace(expression, e);
    }

    void postorder(const IR::ConstructorCallExpression *expression) override {
        const SetOfLocations *result = new SetOfLocations();
        for (auto e : *expression->arguments) {
            auto s = ::P4::get(rw, e->expression);
            CHECK_NULL(s);
            result = result->join(s);
        }
        rw.emplace(expression, result);
    }

    void postorder(const IR::StructExpression *expression) override {
        const SetOfLocations *result = new SetOfLocations();
        for (auto e : expression->components) {
            auto s = ::P4::get(rw, e->expression);
            CHECK_NULL(s);
            result = result->join(s);
        }
        rw.emplace(expression, result);
    }

    void postorder(const IR::ListExpression *expression) override {
        const SetOfLocations *result = new SetOfLocations();
        for (auto e : expression->components) {
            auto s = ::P4::get(rw, e);
            CHECK_NULL(s);
            result = result->join(s);
        }
        rw.emplace(expression, result);
    }

    void postorder(const IR::DefaultExpression *expression) override {
        rw.emplace(expression, new SetOfLocations());
    }

    const SetOfLocations *get(const IR::Expression *expression, const Visitor::Context *ctxt) {
        expression->apply(*this, ctxt);
        auto result = ::P4::get(rw, expression);
        CHECK_NULL(result);
        LOG3("SetOfLocations(" << expression << ")=" << result);
        return result;
    }

    bool mayAlias(const IR::Expression *left, const IR::Expression *right,
                  const Visitor::Context *ctxt) {
        auto llocs = get(left, ctxt);
        auto rlocs = get(right, ctxt);
        CHECK_NULL(llocs);
        CHECK_NULL(rlocs);
        LOG3("Checking overlap between " << llocs << " and " << rlocs);
        return llocs->overlaps(rlocs);
    }
};

}  // namespace P4

#endif /* FRONTENDS_P4_ALIAS_H_ */
