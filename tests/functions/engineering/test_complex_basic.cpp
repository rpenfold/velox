#include <gtest/gtest.h>
#include "velox/formulas/xl-formula.h"

using namespace xl_formula;
using namespace xl_formula::functions;

class ComplexBasicTest : public ::testing::Test {
  protected:
    Context context;

    Value callComplex(const std::vector<Value>& args) {
        return builtin::complex_function(args, context);
    }

    Value callImreal(const std::vector<Value>& args) {
        return builtin::imreal(args, context);
    }

    Value callImaginary(const std::vector<Value>& args) {
        return builtin::imaginary(args, context);
    }
};

// COMPLEX Tests
TEST_F(ComplexBasicTest, Complex_BasicNumbers_CreatesCorrectFormat) {
    auto result = callComplex({Value(3.0), Value(4.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("3+4i", result.asText());

    result = callComplex({Value(5.0), Value(-2.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("5-2i", result.asText());
}

TEST_F(ComplexBasicTest, Complex_ZeroValues_HandlesCorrectly) {
    auto result = callComplex({Value(0.0), Value(0.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("0", result.asText());

    result = callComplex({Value(5.0), Value(0.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("5", result.asText());  // Pure real

    result = callComplex({Value(0.0), Value(3.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("3i", result.asText());  // Pure imaginary
}

TEST_F(ComplexBasicTest, Complex_UnitImaginary_HandlesCorrectly) {
    auto result = callComplex({Value(2.0), Value(1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("2+i", result.asText());  // Should show +i, not +1i

    result = callComplex({Value(2.0), Value(-1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("2-i", result.asText());  // Should show -i, not -1i

    result = callComplex({Value(0.0), Value(1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("i", result.asText());  // Pure unit imaginary

    result = callComplex({Value(0.0), Value(-1.0)});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("-i", result.asText());  // Pure negative unit imaginary
}

TEST_F(ComplexBasicTest, Complex_WithJSuffix_WorksCorrectly) {
    auto result = callComplex({Value(3.0), Value(4.0), Value("j")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("3+4j", result.asText());

    result = callComplex({Value(0.0), Value(1.0), Value("j")});
    ASSERT_TRUE(result.isText());
    EXPECT_EQ("j", result.asText());
}

TEST_F(ComplexBasicTest, Complex_InvalidSuffix_ReturnsError) {
    auto result = callComplex({Value(3.0), Value(4.0), Value("k")});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ComplexBasicTest, Complex_TooFewArguments_ReturnsError) {
    auto result = callComplex({Value(3.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callComplex({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

TEST_F(ComplexBasicTest, Complex_TooManyArguments_ReturnsError) {
    auto result = callComplex({Value(3.0), Value(4.0), Value("i"), Value(5.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// IMREAL Tests
TEST_F(ComplexBasicTest, Imreal_ComplexNumbers_ExtractsRealPart) {
    auto result = callImreal({Value("3+4i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callImreal({Value("5-2j")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(5.0, result.asNumber());

    result = callImreal({Value("-3+4i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-3.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imreal_PureReal_ReturnsRealValue) {
    auto result = callImreal({Value("7")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(7.0, result.asNumber());

    result = callImreal({Value("-5")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-5.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imreal_PureImaginary_ReturnsZero) {
    auto result = callImreal({Value("3i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());

    result = callImreal({Value("i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());

    result = callImreal({Value("-4j")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imreal_TooFewArguments_ReturnsError) {
    auto result = callImreal({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// IMAGINARY Tests
TEST_F(ComplexBasicTest, Imaginary_ComplexNumbers_ExtractsImaginaryPart) {
    auto result = callImaginary({Value("3+4i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(4.0, result.asNumber());

    result = callImaginary({Value("5-2j")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-2.0, result.asNumber());

    result = callImaginary({Value("-3-4i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imaginary_PureReal_ReturnsZero) {
    auto result = callImaginary({Value("7")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());

    result = callImaginary({Value("-5")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imaginary_PureImaginary_ReturnsImaginaryValue) {
    auto result = callImaginary({Value("3i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());

    result = callImaginary({Value("i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());

    result = callImaginary({Value("-i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());

    result = callImaginary({Value("-4j")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-4.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imaginary_UnitImaginary_ReturnsOne) {
    auto result = callImaginary({Value("2+i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(1.0, result.asNumber());

    result = callImaginary({Value("2-i")});
    ASSERT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(-1.0, result.asNumber());
}

TEST_F(ComplexBasicTest, Imaginary_TooFewArguments_ReturnsError) {
    auto result = callImaginary({});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());
}

// Error propagation tests
TEST_F(ComplexBasicTest, AllFunctions_ErrorInput_PropagatesError) {
    auto result = callComplex({Value::error(ErrorType::VALUE_ERROR), Value(4.0)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.asError());

    result = callImreal({Value::error(ErrorType::DIV_ZERO)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.asError());

    result = callImaginary({Value::error(ErrorType::NUM_ERROR)});
    EXPECT_TRUE(result.isError());
    EXPECT_EQ(ErrorType::NUM_ERROR, result.asError());
}
