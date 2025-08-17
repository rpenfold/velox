#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class QuartileFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callQuartile(const std::vector<Value>& args) {
        return builtin::quartile(args, context);
    }
};

TEST_F(QuartileFunctionTest, EmptyArguments_ReturnsError) {
    EXPECT_THROW(callQuartile({}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, SingleArgument_ReturnsError) {
    EXPECT_THROW(callQuartile({Value(5.0)}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, ThreeArguments_ReturnsError) {
    EXPECT_THROW(callQuartile({Value(1.0), Value(2.0), Value(3.0)}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, NonNumericQuart_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callQuartile({Value::array(array), Value("text")}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, NegativeQuart_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callQuartile({Value::array(array), Value(-1.0)}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, QuartGreaterThanFour_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callQuartile({Value::array(array), Value(5.0)}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, FractionalQuart_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callQuartile({Value::array(array), Value(1.5)}), std::invalid_argument);
}

TEST_F(QuartileFunctionTest, SingleValue_AllQuartiles) {
    std::vector<Value> array = {Value(42.0)};
    
    // All quartiles should return the same value for single element
    auto result0 = callQuartile({Value::array(array), Value(0.0)});
    auto result1 = callQuartile({Value::array(array), Value(1.0)});
    auto result2 = callQuartile({Value::array(array), Value(2.0)});
    auto result3 = callQuartile({Value::array(array), Value(3.0)});
    auto result4 = callQuartile({Value::array(array), Value(4.0)});
    
    EXPECT_TRUE(result0.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result0.asNumber());
    EXPECT_TRUE(result1.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result1.asNumber());
    EXPECT_TRUE(result2.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result2.asNumber());
    EXPECT_TRUE(result3.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result3.asNumber());
    EXPECT_TRUE(result4.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result4.asNumber());
}

TEST_F(QuartileFunctionTest, FiveValues_Minimum) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(QuartileFunctionTest, FiveValues_FirstQuartile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 25th percentile
}

TEST_F(QuartileFunctionTest, FiveValues_Median) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 50th percentile (median)
}

TEST_F(QuartileFunctionTest, FiveValues_ThirdQuartile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(3.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // 75th percentile
}

TEST_F(QuartileFunctionTest, FiveValues_Maximum) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(4.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(QuartileFunctionTest, UnsortedArray) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // Median
}

TEST_F(QuartileFunctionTest, DuplicateValues) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callQuartile({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // Median
}

TEST_F(QuartileFunctionTest, NegativeNumbers) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callQuartile({Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-8.0, result.asNumber());  // Minimum
}

TEST_F(QuartileFunctionTest, MixedPositiveNegative) {
    std::vector<Value> array = {Value(-2.0), Value(3.0), Value(-1.0), Value(1.0)};
    auto result = callQuartile({Value::array(array), Value(4.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // Maximum
}

TEST_F(QuartileFunctionTest, WithNonNumericValues) {
    std::vector<Value> array = {Value(1.0), Value("text"), Value(3.0), Value(true), Value(2.0)};
    auto result = callQuartile({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // Median of {1, 3, 2} sorted = {1, 2, 3}
}

TEST_F(QuartileFunctionTest, SingleNonArrayValue) {
    auto result = callQuartile({Value(42.0), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(QuartileFunctionTest, EvenNumberOfValues) {
    std::vector<Value> array = {Value(10.0), Value(20.0), Value(30.0), Value(40.0)};
    auto result = callQuartile({Value::array(array), Value(2.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(25.0, result.asNumber());  // Interpolated median
}

TEST_F(QuartileFunctionTest, AllNonNumericValues_ReturnsError) {
    std::vector<Value> array = {Value("text"), Value(true), Value::error(ErrorType::VALUE_ERROR)};
    EXPECT_THROW(callQuartile({Value::array(array), Value(2.0)}), std::invalid_argument);
}