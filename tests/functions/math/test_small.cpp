#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class SmallFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callSmall(const std::vector<Value>& args) {
        return builtin::small(args, context);
    }
};

TEST_F(SmallFunctionTest, EmptyArguments_ReturnsError) {
    EXPECT_THROW(callSmall({}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, SingleArgument_ReturnsError) {
    EXPECT_THROW(callSmall({Value(5.0)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, ThreeArguments_ReturnsError) {
    EXPECT_THROW(callSmall({Value(1.0), Value(2.0), Value(3.0)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, NonNumericK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callSmall({Value::array(array), Value("text")}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, ZeroK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callSmall({Value::array(array), Value(0.0)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, NegativeK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callSmall({Value::array(array), Value(-1.0)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, FractionalK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callSmall({Value::array(array), Value(1.5)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, KTooLarge_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callSmall({Value::array(array), Value(5.0)}), std::invalid_argument);
}

TEST_F(SmallFunctionTest, SingleValue_FirstSmallest) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(SmallFunctionTest, TwoValues_FirstSmallest) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SmallFunctionTest, TwoValues_SecondSmallest) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callSmall({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(SmallFunctionTest, FiveValues_FirstSmallest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SmallFunctionTest, FiveValues_SecondSmallest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(SmallFunctionTest, FiveValues_ThirdSmallest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(3.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(SmallFunctionTest, UnsortedArray) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(SmallFunctionTest, DuplicateValues) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SmallFunctionTest, DuplicateValues_SecondSmallest) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callSmall({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(SmallFunctionTest, NegativeNumbers) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-8.0, result.asNumber());
}

TEST_F(SmallFunctionTest, MixedPositiveNegative) {
    std::vector<Value> array = {Value(-2.0), Value(3.0), Value(-1.0), Value(1.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());
}

TEST_F(SmallFunctionTest, WithNonNumericValues) {
    std::vector<Value> array = {Value(1.0), Value("text"), Value(3.0), Value(true), Value(2.0)};
    auto result = callSmall({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(SmallFunctionTest, SingleNonArrayValue) {
    auto result = callSmall({Value(42.0), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(SmallFunctionTest, DecimalNumbers) {
    std::vector<Value> array = {Value(1.5), Value(2.7), Value(3.1), Value(4.9)};
    auto result = callSmall({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.7, result.asNumber());
}

TEST_F(SmallFunctionTest, AllNonNumericValues_ReturnsError) {
    std::vector<Value> array = {Value("text"), Value(true), Value::error(ErrorType::VALUE_ERROR)};
    EXPECT_THROW(callSmall({Value::array(array), Value(1.0)}), std::invalid_argument);
}