#include <gtest/gtest.h>
#include <xl-formula/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class BooleanFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callTrue(const std::vector<Value>& args = {}) {
        return builtin::true_function(args, context);
    }

    Value callFalse(const std::vector<Value>& args = {}) {
        return builtin::false_function(args, context);
    }
};

// TRUE() Function Tests
TEST_F(BooleanFunctionTest, TrueFunction_NoArguments_ReturnsTrue) {
    auto result = callTrue();

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, TrueFunction_WithArguments_IgnoresArgsReturnsTrue) {
    auto result = callTrue({Value(1.0), Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, TrueFunction_WithErrorArguments_IgnoresArgsReturnsTrue) {
    auto result = callTrue({Value::error(ErrorType::DIV_ZERO)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, TrueFunction_WithManyArguments_IgnoresArgsReturnsTrue) {
    auto result = callTrue({Value(1.0), Value("text"), Value(false), Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

// FALSE() Function Tests
TEST_F(BooleanFunctionTest, FalseFunction_NoArguments_ReturnsFalse) {
    auto result = callFalse();

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, FalseFunction_WithArguments_IgnoresArgsReturnsFalse) {
    auto result = callFalse({Value(1.0), Value("hello")});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, FalseFunction_WithErrorArguments_IgnoresArgsReturnsFalse) {
    auto result = callFalse({Value::error(ErrorType::VALUE_ERROR)});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, FalseFunction_WithManyArguments_IgnoresArgsReturnsFalse) {
    auto result = callFalse({Value(1.0), Value("text"), Value(true), Value::empty()});

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}

// Comparison Tests
TEST_F(BooleanFunctionTest, TrueAndFalse_HaveDifferentValues) {
    auto trueResult = callTrue();
    auto falseResult = callFalse();

    EXPECT_TRUE(trueResult.isBoolean());
    EXPECT_TRUE(falseResult.isBoolean());
    EXPECT_NE(trueResult.asBoolean(), falseResult.asBoolean());
}

TEST_F(BooleanFunctionTest, TrueFunction_ConsistentResults) {
    auto result1 = callTrue();
    auto result2 = callTrue();

    EXPECT_EQ(result1.asBoolean(), result2.asBoolean());
    EXPECT_TRUE(result1.asBoolean());
    EXPECT_TRUE(result2.asBoolean());
}

TEST_F(BooleanFunctionTest, FalseFunction_ConsistentResults) {
    auto result1 = callFalse();
    auto result2 = callFalse();

    EXPECT_EQ(result1.asBoolean(), result2.asBoolean());
    EXPECT_FALSE(result1.asBoolean());
    EXPECT_FALSE(result2.asBoolean());
}

// Edge Cases
TEST_F(BooleanFunctionTest, TrueFunction_WithLargeNumberOfArguments_StillWorks) {
    std::vector<Value> manyArgs;
    for (int i = 0; i < 100; ++i) {
        manyArgs.push_back(Value(static_cast<double>(i)));
    }

    auto result = callTrue(manyArgs);

    EXPECT_TRUE(result.isBoolean());
    EXPECT_TRUE(result.asBoolean());
}

TEST_F(BooleanFunctionTest, FalseFunction_WithLargeNumberOfArguments_StillWorks) {
    std::vector<Value> manyArgs;
    for (int i = 0; i < 100; ++i) {
        manyArgs.push_back(Value(static_cast<double>(i)));
    }

    auto result = callFalse(manyArgs);

    EXPECT_TRUE(result.isBoolean());
    EXPECT_FALSE(result.asBoolean());
}