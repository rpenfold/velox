#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class LargeFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callLarge(const std::vector<Value>& args) {
        return builtin::large(args, context);
    }
};

TEST_F(LargeFunctionTest, EmptyArguments_ReturnsError) {
    EXPECT_THROW(callLarge({}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, SingleArgument_ReturnsError) {
    EXPECT_THROW(callLarge({Value(5.0)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, ThreeArguments_ReturnsError) {
    EXPECT_THROW(callLarge({Value(1.0), Value(2.0), Value(3.0)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, NonNumericK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callLarge({Value::array(array), Value("text")}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, ZeroK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callLarge({Value::array(array), Value(0.0)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, NegativeK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callLarge({Value::array(array), Value(-1.0)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, FractionalK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callLarge({Value::array(array), Value(1.5)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, KTooLarge_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callLarge({Value::array(array), Value(5.0)}), std::invalid_argument);
}

TEST_F(LargeFunctionTest, SingleValue_FirstLargest) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(LargeFunctionTest, TwoValues_FirstLargest) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(LargeFunctionTest, TwoValues_SecondLargest) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callLarge({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(LargeFunctionTest, FiveValues_FirstLargest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(LargeFunctionTest, FiveValues_SecondLargest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(LargeFunctionTest, FiveValues_ThirdLargest) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(3.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(LargeFunctionTest, UnsortedArray) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(LargeFunctionTest, DuplicateValues) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(LargeFunctionTest, DuplicateValues_SecondLargest) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callLarge({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(LargeFunctionTest, NegativeNumbers) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(LargeFunctionTest, MixedPositiveNegative) {
    std::vector<Value> array = {Value(-2.0), Value(3.0), Value(-1.0), Value(1.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(LargeFunctionTest, WithNonNumericValues) {
    std::vector<Value> array = {Value(1.0), Value("text"), Value(3.0), Value(true), Value(2.0)};
    auto result = callLarge({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(LargeFunctionTest, SingleNonArrayValue) {
    auto result = callLarge({Value(42.0), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(LargeFunctionTest, DecimalNumbers) {
    std::vector<Value> array = {Value(1.5), Value(2.7), Value(3.1), Value(4.9)};
    auto result = callLarge({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.1, result.asNumber());
}

TEST_F(LargeFunctionTest, AllNonNumericValues_ReturnsError) {
    std::vector<Value> array = {Value("text"), Value(true), Value::error(ErrorType::VALUE_ERROR)};
    EXPECT_THROW(callLarge({Value::array(array), Value(1.0)}), std::invalid_argument);
}