#include <gtest/gtest.h>
#include <xl-formula/types.h>
#include <algorithm>

using namespace xl_formula;

class ValueTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ValueTest, NumberConstructorAndAccessors) {
    Value num(42.5);

    EXPECT_TRUE(num.isNumber());
    EXPECT_FALSE(num.isText());
    EXPECT_FALSE(num.isBoolean());
    EXPECT_FALSE(num.isDate());
    EXPECT_FALSE(num.isError());
    EXPECT_FALSE(num.isEmpty());

    EXPECT_EQ(ValueType::NUMBER, num.getType());
    EXPECT_DOUBLE_EQ(42.5, num.asNumber());
    EXPECT_TRUE(num.canConvertToNumber());
    EXPECT_DOUBLE_EQ(42.5, num.toNumber());
    EXPECT_EQ("42.5", num.toString());
}

TEST_F(ValueTest, TextConstructorAndAccessors) {
    Value text("Hello World");

    EXPECT_FALSE(text.isNumber());
    EXPECT_TRUE(text.isText());
    EXPECT_FALSE(text.isBoolean());
    EXPECT_FALSE(text.isDate());
    EXPECT_FALSE(text.isError());
    EXPECT_FALSE(text.isEmpty());

    EXPECT_EQ(ValueType::TEXT, text.getType());
    EXPECT_EQ("Hello World", text.asText());
    EXPECT_FALSE(text.canConvertToNumber());
    EXPECT_EQ("Hello World", text.toString());
}

TEST_F(ValueTest, BooleanConstructorAndAccessors) {
    Value boolean_true(true);
    Value boolean_false(false);

    EXPECT_TRUE(boolean_true.isBoolean());
    EXPECT_TRUE(boolean_false.isBoolean());

    EXPECT_TRUE(boolean_true.asBoolean());
    EXPECT_FALSE(boolean_false.asBoolean());

    EXPECT_TRUE(boolean_true.canConvertToNumber());
    EXPECT_TRUE(boolean_false.canConvertToNumber());
    EXPECT_DOUBLE_EQ(1.0, boolean_true.toNumber());
    EXPECT_DOUBLE_EQ(0.0, boolean_false.toNumber());

    EXPECT_EQ("TRUE", boolean_true.toString());
    EXPECT_EQ("FALSE", boolean_false.toString());
}

TEST_F(ValueTest, ErrorConstructorAndAccessors) {
    Value error(ErrorType::DIV_ZERO);

    EXPECT_FALSE(error.isNumber());
    EXPECT_FALSE(error.isText());
    EXPECT_FALSE(error.isBoolean());
    EXPECT_FALSE(error.isDate());
    EXPECT_TRUE(error.isError());
    EXPECT_FALSE(error.isEmpty());

    EXPECT_EQ(ValueType::ERROR, error.getType());
    EXPECT_EQ(ErrorType::DIV_ZERO, error.asError());
    EXPECT_EQ("#DIV/0!", error.toString());
}

TEST_F(ValueTest, EmptyConstructorAndAccessors) {
    Value empty;

    EXPECT_FALSE(empty.isNumber());
    EXPECT_FALSE(empty.isText());
    EXPECT_FALSE(empty.isBoolean());
    EXPECT_FALSE(empty.isDate());
    EXPECT_FALSE(empty.isError());
    EXPECT_TRUE(empty.isEmpty());

    EXPECT_EQ(ValueType::EMPTY, empty.getType());
    EXPECT_EQ("", empty.toString());
}

TEST_F(ValueTest, NumericTextConversion) {
    Value numeric_text("123.45");

    EXPECT_TRUE(numeric_text.isText());
    EXPECT_TRUE(numeric_text.canConvertToNumber());
    EXPECT_DOUBLE_EQ(123.45, numeric_text.toNumber());
}

TEST_F(ValueTest, NonNumericTextConversion) {
    Value non_numeric_text("abc123");

    EXPECT_TRUE(non_numeric_text.isText());
    EXPECT_FALSE(non_numeric_text.canConvertToNumber());
    EXPECT_THROW(non_numeric_text.toNumber(), std::runtime_error);
}

TEST_F(ValueTest, ComparisonOperators) {
    Value num1(10.0);
    Value num2(20.0);
    Value num3(10.0);

    EXPECT_TRUE(num1 == num3);
    EXPECT_FALSE(num1 == num2);
    EXPECT_TRUE(num1 != num2);
    EXPECT_FALSE(num1 != num3);

    EXPECT_TRUE(num1 < num2);
    EXPECT_FALSE(num2 < num1);
    EXPECT_TRUE(num1 <= num2);
    EXPECT_TRUE(num1 <= num3);

    EXPECT_TRUE(num2 > num1);
    EXPECT_FALSE(num1 > num2);
    EXPECT_TRUE(num2 >= num1);
    EXPECT_TRUE(num3 >= num1);
}

TEST_F(ValueTest, StaticFactoryMethods) {
    Value error = Value::error(ErrorType::VALUE_ERROR);
    Value empty = Value::empty();

    EXPECT_TRUE(error.isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, error.asError());

    EXPECT_TRUE(empty.isEmpty());
}

class ContextTest : public ::testing::Test {
  protected:
    Context context;

    void SetUp() override {
        context.setVariable("A1", Value(10.0));
        context.setVariable("A2", Value("Hello"));
        context.setVariable("A3", Value(true));
    }
};

TEST_F(ContextTest, SetAndGetVariable) {
    EXPECT_TRUE(context.hasVariable("A1"));
    EXPECT_TRUE(context.hasVariable("A2"));
    EXPECT_TRUE(context.hasVariable("A3"));
    EXPECT_FALSE(context.hasVariable("B1"));

    Value a1 = context.getVariable("A1");
    EXPECT_TRUE(a1.isNumber());
    EXPECT_DOUBLE_EQ(10.0, a1.asNumber());

    Value a2 = context.getVariable("A2");
    EXPECT_TRUE(a2.isText());
    EXPECT_EQ("Hello", a2.asText());

    Value a3 = context.getVariable("A3");
    EXPECT_TRUE(a3.isBoolean());
    EXPECT_TRUE(a3.asBoolean());

    Value b1 = context.getVariable("B1");
    EXPECT_TRUE(b1.isEmpty());
}

TEST_F(ContextTest, RemoveVariable) {
    EXPECT_TRUE(context.hasVariable("A1"));

    context.removeVariable("A1");
    EXPECT_FALSE(context.hasVariable("A1"));

    Value a1 = context.getVariable("A1");
    EXPECT_TRUE(a1.isEmpty());
}

TEST_F(ContextTest, GetVariableNames) {
    auto names = context.getVariableNames();
    EXPECT_EQ(3, names.size());

    // Names should include A1, A2, A3 (order not guaranteed)
    std::sort(names.begin(), names.end());
    EXPECT_EQ("A1", names[0]);
    EXPECT_EQ("A2", names[1]);
    EXPECT_EQ("A3", names[2]);
}

TEST_F(ContextTest, ClearVariables) {
    EXPECT_EQ(3, context.getVariableNames().size());

    context.clear();
    EXPECT_EQ(0, context.getVariableNames().size());
    EXPECT_FALSE(context.hasVariable("A1"));
    EXPECT_FALSE(context.hasVariable("A2"));
    EXPECT_FALSE(context.hasVariable("A3"));
}