#include <gtest/gtest.h>
#include "xl-formula/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class IfsFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    Value callIfs(const std::vector<Value>& args) {
        return builtin::ifs_function(args, context);
    }
    
    // Helper function to create a date
    Value::DateType makeDate(int year, int month, int day) {
        std::tm time_info = {};
        time_info.tm_year = year - 1900;
        time_info.tm_mon = month - 1;
        time_info.tm_mday = day;
        time_info.tm_hour = 0;
        time_info.tm_min = 0;
        time_info.tm_sec = 0;
        
        return std::chrono::system_clock::from_time_t(std::mktime(&time_info));
    }
};

TEST_F(IfsFunctionTest, TooFewArguments_ReturnsError) {
    auto result = callIfs({});
    EXPECT_TRUE(result.isError());
    
    result = callIfs({Value(true)});
    EXPECT_TRUE(result.isError());
}

TEST_F(IfsFunctionTest, OddNumberOfArguments_ReturnsError) {
    auto result = callIfs({Value(true), Value("Result"), Value(false)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfsFunctionTest, FirstConditionTrue_ReturnsFirstResult) {
    auto result = callIfs({Value(true), Value("First"), Value(false), Value("Second")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("First", result.asText());
}

TEST_F(IfsFunctionTest, SecondConditionTrue_ReturnsSecondResult) {
    auto result = callIfs({Value(false), Value("First"), Value(true), Value("Second")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Second", result.asText());
}

TEST_F(IfsFunctionTest, NoConditionTrue_ReturnsNA) {
    auto result = callIfs({Value(false), Value("First"), Value(false), Value("Second")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NA_ERROR, result.asError());
}

TEST_F(IfsFunctionTest, NumberConditions_ZeroIsFalse) {
    auto result = callIfs({Value(0.0), Value("Zero"), Value(1.0), Value("One")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("One", result.asText());
}

TEST_F(IfsFunctionTest, NumberConditions_NonZeroIsTrue) {
    auto result = callIfs({Value(5.0), Value("Five"), Value(false), Value("False")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Five", result.asText());
    
    result = callIfs({Value(-3.0), Value("Negative"), Value(false), Value("False")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Negative", result.asText());
}

TEST_F(IfsFunctionTest, TextConditions_ReturnsError) {
    auto result = callIfs({Value("true"), Value("Text"), Value(true), Value("Boolean")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfsFunctionTest, ErrorCondition_PropagatesError) {
    auto result = callIfs({Value::error(ErrorType::DIV_ZERO), Value("Error"), Value(true), Value("True")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());
}

TEST_F(IfsFunctionTest, ErrorInResult_PropagatesOnMatch) {
    auto result = callIfs({Value(true), Value::error(ErrorType::VALUE_ERROR), Value(false), Value("False")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(IfsFunctionTest, EmptyCondition_IsFalse) {
    auto result = callIfs({Value::empty(), Value("Empty"), Value(true), Value("True")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("True", result.asText());
}

TEST_F(IfsFunctionTest, DateCondition_IsTrue) {
    auto date_val = makeDate(2024, 1, 1);
    auto result = callIfs({Value(date_val), Value("Date"), Value(false), Value("False")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Date", result.asText());
}

TEST_F(IfsFunctionTest, MultipleConditions_ReturnsFirstTrue) {
    auto result = callIfs({Value(false), Value("First"),
                           Value(true), Value("Second"),
                           Value(true), Value("Third")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Second", result.asText());
}

TEST_F(IfsFunctionTest, ComplexResultTypes_ReturnsCorrectType) {
    // Test returning numbers
    auto result = callIfs({Value(true), Value(42.0), Value(false), Value(0.0)});
    ASSERT_TRUE(result.isNumber());
    EXPECT_EQ(42.0, result.asNumber());
    
    // Test returning booleans
    result = callIfs({Value(true), Value(false), Value(true), Value(true)});
    ASSERT_TRUE(result.isBoolean());
    EXPECT_EQ(false, result.asBoolean());
}

TEST_F(IfsFunctionTest, ManyConditions_WorksCorrectly) {
    std::vector<Value> args;
    for (int i = 1; i <= 10; ++i) {
        args.push_back(Value(false));  // condition
        args.push_back(Value("Result" + std::to_string(i)));  // result
    }
    // Add one true condition at the end
    args.push_back(Value(true));
    args.push_back(Value("Final"));
    
    auto result = callIfs(args);
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Final", result.asText());
}

TEST_F(IfsFunctionTest, BooleanLiterals_WorkCorrectly) {
    auto result = callIfs({Value(false), Value("False"), Value(true), Value("True")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("True", result.asText());
}

TEST_F(IfsFunctionTest, NegativeNumbers_TreatAsTrue) {
    auto result = callIfs({Value(-1.0), Value("Negative"), Value(false), Value("False")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Negative", result.asText());
}

TEST_F(IfsFunctionTest, FloatingPointNumbers_WorkCorrectly) {
    auto result = callIfs({Value(0.0), Value("Zero"), 
                           Value(0.1), Value("PointOne"), 
                           Value(false), Value("False")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("PointOne", result.asText());
}

TEST_F(IfsFunctionTest, SingleCondition_WorksCorrectly) {
    auto result = callIfs({Value(true), Value("Only")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("Only", result.asText());
    
    result = callIfs({Value(false), Value("Only")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NA_ERROR, result.asError());
}
