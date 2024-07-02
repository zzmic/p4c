#include "backends/p4tools/modules/smith/common/declarations.h"
#include "backends/p4tools/modules/smith/common/expressions.h"
#include "backends/p4tools/modules/smith/common/generator.h"
#include "backends/p4tools/modules/smith/common/parser.h"
#include "backends/p4tools/modules/smith/common/probabilities.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/statements.h"
#include "backends/p4tools/modules/smith/common/table.h"
#include "backends/p4tools/modules/smith/core/target.h"
#include "backends/p4tools/modules/smith/smith.h"
#include "gtest/gtest.h"
#include "ir/ir.h"

namespace Test {

class P4SmithForInLoopTest : public ::testing::Test {
 protected:
    P4Tools::P4Smith::StatementGenerator *generator;

    // TODO(zzmic): Figure out how to properly initialize and clean up the test object.
    P4SmithForInLoopTest() {}
    ~P4SmithForInLoopTest() override {}
};

/// @brief Test the generation of a for-in-loop statement.
TEST_F(P4SmithForInLoopTest, ForLoopGeneration) {
    auto forInLoopStmt = generator->genForInLoopStatement(false);
    ASSERT_NE(forInLoopStmt, nullptr);
    EXPECT_TRUE(forInLoopStmt->is<IR::ForInStatement>());
}

/// @brief Test the for-in-loop's declaration variable.
TEST_F(P4SmithForInLoopTest, CheckForLoopDeclarationVariable) {
    auto forInLoopStmt = generator->genForInLoopStatement(false);
    ASSERT_NE(forInLoopStmt, nullptr);
    EXPECT_TRUE(forInLoopStmt->is<IR::ForInStatement>());

    // TODO(zzmic): Figure out whether this "static_cast" is necessary.
    auto forInStmt = forInLoopStmt->to<IR::ForInStatement>();
    ASSERT_NE(forInStmt->decl, nullptr);
    EXPECT_TRUE(forInStmt->decl->is<IR::Declaration_Variable>());
}

/// @brief Test the for-in-loop's collection.
TEST_F(P4SmithForInLoopTest, CheckForLoopCollection) {
    auto forInLoopStmt = generator->genForInLoopStatement(false);
    ASSERT_NE(forInLoopStmt, nullptr);
    EXPECT_TRUE(forInLoopStmt->is<IR::ForInStatement>());

    // TODO(zzmic): Figure out whether this "static_cast" is necessary.
    auto forInStmt = forInLoopStmt->to<IR::ForInStatement>();
    ASSERT_NE(forInStmt, nullptr);
    EXPECT_TRUE(forInStmt->collection->is<IR::PathExpression>());
}

/// @brief Test the for-in-loop's body.
TEST_F(P4SmithForInLoopTest, CheckForLoopBody) {
    auto forInLoopStmt = generator->genForInLoopStatement(false);
    ASSERT_NE(forInLoopStmt, nullptr);
    EXPECT_TRUE(forInLoopStmt->is<IR::ForInStatement>());

    // TODO(zzmic): Figure out whether this "static_cast" is necessary.
    auto forInStmt = forInLoopStmt->to<IR::ForInStatement>();
    ASSERT_NE(forInStmt->body, nullptr);
    EXPECT_FALSE(forInStmt->body->is<IR::Statement>());
}

}  // namespace Test
