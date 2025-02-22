/*
Copyright 2017 VMware, Inc.

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

#ifndef FRONTENDS_P4_SETHEADERS_H_
#define FRONTENDS_P4_SETHEADERS_H_

#include "frontends/common/resolveReferences/resolveReferences.h"
#include "frontends/p4/typeChecking/typeChecker.h"
#include "ir/ir.h"

namespace P4 {
/**
Assigning a list expression to a header should also
set the header validity bit.  For example, given:

header H { ... };
struct S { H h; }
S s;

The following fragment:

s = { { 1, 2 } };

is converted to:

s.h.setValid();
s = { { 1, 2 } };

@pre This pass should be run after RemoveInitializers - it only looks
at assignment statements.  It should also run after
SideEffectOrdering, because that pass inserts temporaries for the case
of tuples passed as arguments to functions expecting headers, reducing
them to assignments.
*/
class DoSetHeaders final : public Transform {
    TypeMap *typeMap;

    bool containsHeaderType(const IR::Type *type);
    void generateSetValid(const IR::Expression *dest, const IR::Expression *src,
                          const IR::Type *destType, IR::Vector<IR::StatOrDecl> &insert);

 public:
    explicit DoSetHeaders(TypeMap *typeMap) : typeMap(typeMap) {
        CHECK_NULL(typeMap);
        setName("DoSetHeaders");
    }
    const IR::Node *postorder(IR::AssignmentStatement *assign) override;
};

class SetHeaders final : public PassManager {
 public:
    explicit SetHeaders(TypeMap *typeMap) {
        passes.push_back(new P4::TypeChecking(nullptr, typeMap));
        passes.push_back(new P4::DoSetHeaders(typeMap));
        setName("SetHeaders");
    }
};

}  // namespace P4

#endif /* FRONTENDS_P4_SETHEADERS_H_ */
