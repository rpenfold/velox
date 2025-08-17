#include <gtest/gtest.h>
#include <velox/formulas/functions.h>

using namespace xl_formula;
using namespace xl_formula::functions;

class RankFunctionTest : public ::testing::Test {
  protected:
    Context context;

    Value callRank(const std::vector<Value>& args) {
        return builtin::rank(args, context);
    }
};

TEST_F(RankFunctionTest, EmptyArguments_ReturnsError) {
    EXPECT_THROW(callRank({}), std::invalid_argument);
}

TEST_F(RankFunctionTest, SingleArgument_ReturnsError) {
    EXPECT_THROW(callRank({Value(5.0)}), std::invalid_argument);
}

TEST_F(RankFunctionTest, FourArguments_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callRank({Value(2.0), Value::array(array), Value(1.0), Value(0.0)}), std::invalid_argument);
}

TEST_F(RankFunctionTest, NonNumericNumber_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callRank({Value("text"), Value::array(array)}), std::invalid_argument);
}

TEST_F(RankFunctionTest, NonNumericOrder_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callRank({Value(2.0), Value::array(array), Value("text")}), std::invalid_argument);
}

TEST_F(RankFunctionTest, InvalidOrder_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callRank({Value(2.0), Value::array(array), Value(2.0)}), std::invalid_argument);
}

TEST_F(RankFunctionTest, NumberNotInArray_ReturnsError) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0)};
    EXPECT_THROW(callRank({Value(5.0), Value::array(array)}), std::invalid_argument);
}

TEST_F(RankFunctionTest, SingleValue_DefaultOrder) {
    std::vector<Value> array = {Value(42.0)};
    auto result = callRank({Value(42.0), Value::array(array)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(RankFunctionTest, TwoValues_DefaultOrder_FirstValue) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callRank({Value(3.0), Value::array(array)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());  // 3 is largest in descending order
}

TEST_F(RankFunctionTest, TwoValues_DefaultOrder_SecondValue) {
    std::vector<Value> array = {Value(1.0), Value(3.0)};
    auto result = callRank({Value(1.0), Value::array(array)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 1 is smallest in descending order
}

TEST_F(RankFunctionTest, FiveValues_DescendingOrder) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callRank({Value(3.0), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 3 is 3rd largest (5,4,3,2,1)
}

TEST_F(RankFunctionTest, FiveValues_AscendingOrder) {
    std::vector<Value> array = {Value(1.0), Value(2.0), Value(3.0), Value(4.0), Value(5.0)};
    auto result = callRank({Value(3.0), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 3 is 3rd smallest (1,2,3,4,5)
}

TEST_F(RankFunctionTest, UnsortedArray_DescendingOrder) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callRank({Value(4.0), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 4 is 2nd largest (5,4,3,2,1)
}

TEST_F(RankFunctionTest, UnsortedArray_AscendingOrder) {
    std::vector<Value> array = {Value(3.0), Value(1.0), Value(4.0), Value(2.0), Value(5.0)};
    auto result = callRank({Value(4.0), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());  // 4 is 4th smallest (1,2,3,4,5)
}

TEST_F(RankFunctionTest, DuplicateValues_DescendingOrder) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callRank({Value(3.0), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // First occurrence of 3 gets rank 2
}

TEST_F(RankFunctionTest, DuplicateValues_AscendingOrder) {
    std::vector<Value> array = {Value(1.0), Value(3.0), Value(3.0), Value(5.0)};
    auto result = callRank({Value(3.0), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // First occurrence of 3 gets rank 2
}

TEST_F(RankFunctionTest, NegativeNumbers_DescendingOrder) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callRank({Value(-2.0), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // -2 is 2nd largest (-1,-2,-5,-8)
}

TEST_F(RankFunctionTest, NegativeNumbers_AscendingOrder) {
    std::vector<Value> array = {Value(-5.0), Value(-2.0), Value(-8.0), Value(-1.0)};
    auto result = callRank({Value(-2.0), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // -2 is 3rd smallest (-8,-5,-2,-1)
}

TEST_F(RankFunctionTest, MixedPositiveNegative_DescendingOrder) {
    std::vector<Value> array = {Value(-2.0), Value(3.0), Value(-1.0), Value(1.0)};
    auto result = callRank({Value(1.0), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 1 is 2nd largest (3,1,-1,-2)
}

TEST_F(RankFunctionTest, WithNonNumericValues) {
    std::vector<Value> array = {Value(1.0), Value("text"), Value(3.0), Value(true), Value(2.0)};
    auto result = callRank({Value(2.0), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // Among {1,3,2}, 2 is 2nd smallest
}

TEST_F(RankFunctionTest, SingleNonArrayValue) {
    auto result = callRank({Value(42.0), Value(42.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());
}

TEST_F(RankFunctionTest, DecimalNumbers_DescendingOrder) {
    std::vector<Value> array = {Value(1.5), Value(2.7), Value(3.1), Value(4.9)};
    auto result = callRank({Value(2.7), Value::array(array), Value(0.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());  // 2.7 is 3rd largest (4.9,3.1,2.7,1.5)
}

TEST_F(RankFunctionTest, DecimalNumbers_AscendingOrder) {
    std::vector<Value> array = {Value(1.5), Value(2.7), Value(3.1), Value(4.9)};
    auto result = callRank({Value(2.7), Value::array(array), Value(1.0)});
    
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());  // 2.7 is 2nd smallest (1.5,2.7,3.1,4.9)
}

TEST_F(RankFunctionTest, AllNonNumericValues_ReturnsError) {
    std::vector<Value> array = {Value("text"), Value(true), Value::error(ErrorType::VALUE_ERROR)};
    EXPECT_THROW(callRank({Value(1.0), Value::array(array)}), std::invalid_argument);
}