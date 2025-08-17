#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class PercentileFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callPercentile(const std::vector<Value>& args) {
        return builtin::percentile(args, context);
    }
};

TEST_F(PercentileFunctionTest, EmptyArguments_ReturnsError) {
    EXPECT_THROW(callPercentile({}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, SingleArgument_ReturnsError) {
    EXPECT_THROW(callPercentile({Value(5.0)}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, ThreeArguments_ReturnsError) {
    EXPECT_THROW(callPercentile({Value(1.0), Value(2.0), Value(3.0)}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, NonNumericK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callPercentile({Value::array(array), Value("text")}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, NegativeK_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callPercentile({Value::array(array), Value(-0.1)}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, KGreaterThanOne_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callPercentile({Value::array(array), Value(1.1)}), std::invalid_argument);
}

TEST_F(PercentileFunctionTest, SingleValue_ZeroPercentile) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callPercentile({Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, SingleValue_FiftyPercentile) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, SingleValue_OneHundredPercentile) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callPercentile({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, FiveValues_ZeroPercentile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, FiveValues_FiftyPercentile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, FiveValues_OneHundredPercentile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, FiveValues_TwentyFivePercentile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.25)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, FiveValues_SeventyFivePercentile) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.75)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, UnsortedArray) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, DuplicateValues) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, NegativeNumbers) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.5, result.asNumber());  // Interpolation between -5 and -2
}

TEST_F(PercentileFunctionTest, WithNonNumericValues) {
    std::vector<Value> array = {Value(1.0), Value("text"), Value(3.0), Value(true), Value(2.0)};
    auto result = callPercentile({Value::array(array), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // Median of {1, 3, 2} sorted = {1, 2, 3}
}

TEST_F(PercentileFunctionTest, SingleNonArrayValue) {
    auto result = callPercentile({Value(42.0), Value(0.5)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(42.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, InterpolationNeeded) {
    std::vector<Value> array = {Value(10.0), Value(20.0), Value(30.0), Value(40.0)};
    auto result = callPercentile({Value::array(array), Value(0.3)});
    
    EXPECT_TRUE(result.isNumber());
    // Position = 0.3 * (4-1) = 0.9, interpolation between index 0 (10) and index 1 (20)
    // Result = 10 + 0.9 * (20-10) = 19
    EXPECT_DOUBLE_EQ(19.0, result.asNumber());
}

TEST_F(PercentileFunctionTest, AllNonNumericValues_ReturnsError) {
    std::vector<Value> array = {Value("text"), Value(true), Value::error(ErrorType::VALUE_ERROR)};
    EXPECT_THROW(callPercentile({Value::array(array), Value(0.5)}), std::invalid_argument);
}