#include <gtest/gtest.h>
#include <xl-formula/evaluator.h>
#include <xl-formula/parser.h>

using namespace xl_formula;

class EvaluatorTest : public ::testing::Test {
  protected:
    Context context;
    std::unique_ptr<FunctionRegistry> registry;

    void SetUp() override {
        registry = FunctionRegistry::createDefault();

        // Set up test variables
        context.setVariable("A1", Value(10.0));
        context.setVariable("A2", Value(20.0));
        context.setVariable("A3", Value(30.0));
        context.setVariable("B1", Value("Hello"));
        context.setVariable("B2", Value("World"));
        context.setVariable("C1", Value(true));
        context.setVariable("C2", Value(false));
    }

    Value evaluateFormula(const std::string& formula) {
        Parser parser;
        auto parse_result = parser.parse(formula);
        EXPECT_TRUE(parse_result.isSuccess()) << "Parse failed for: " << formula;

        if (!parse_result.isSuccess()) {
            return Value::error(ErrorType::PARSE_ERROR);
        }

        Evaluator evaluator(context, registry.get());
        auto eval_result = evaluator.evaluate(*parse_result.getAST());
        EXPECT_TRUE(eval_result.isSuccess()) << "Evaluation failed for: " << formula;

        return eval_result.getValue();
    }

    void checkNumberResult(const std::string& formula, double expected) {
        Value result = evaluateFormula(formula);
        EXPECT_TRUE(result.isNumber()) << "Expected number result for: " << formula;
        if (result.isNumber()) {
            EXPECT_DOUBLE_EQ(expected, result.asNumber()) << "Formula: " << formula;
        }
    }

    void checkTextResult(const std::string& formula, const std::string& expected) {
        Value result = evaluateFormula(formula);
        EXPECT_TRUE(result.isText()) << "Expected text result for: " << formula;
        if (result.isText()) {
            EXPECT_EQ(expected, result.asText()) << "Formula: " << formula;
        }
    }

    void checkBooleanResult(const std::string& formula, bool expected) {
        Value result = evaluateFormula(formula);
        EXPECT_TRUE(result.isBoolean()) << "Expected boolean result for: " << formula;
        if (result.isBoolean()) {
            EXPECT_EQ(expected, result.asBoolean()) << "Formula: " << formula;
        }
    }

    void checkErrorResult(const std::string& formula, ErrorType expected_error) {
        Value result = evaluateFormula(formula);
        EXPECT_TRUE(result.isError()) << "Expected error result for: " << formula;
        if (result.isError()) {
            EXPECT_EQ(expected_error, result.asError()) << "Formula: " << formula;
        }
    }
};

TEST_F(EvaluatorTest, Literals) {
    checkNumberResult("42", 42.0);
    checkNumberResult("123.45", 123.45);
    checkTextResult("\"hello\"", "hello");
    checkBooleanResult("TRUE", true);
    checkBooleanResult("FALSE", false);
}

TEST_F(EvaluatorTest, Variables) {
    checkNumberResult("A1", 10.0);
    checkNumberResult("A2", 20.0);
    checkTextResult("B1", "Hello");
    checkBooleanResult("C1", true);

    // Non-existent variable should return error
    checkErrorResult("X1", ErrorType::NAME_ERROR);
}

TEST_F(EvaluatorTest, BasicArithmetic) {
    checkNumberResult("1 + 2", 3.0);
    checkNumberResult("10 - 5", 5.0);
    checkNumberResult("3 * 4", 12.0);
    checkNumberResult("15 / 3", 5.0);
    checkNumberResult("2 ^ 3", 8.0);
}

TEST_F(EvaluatorTest, ArithmeticWithVariables) {
    checkNumberResult("A1 + A2", 30.0);   // 10 + 20
    checkNumberResult("A3 - A1", 20.0);   // 30 - 10
    checkNumberResult("A1 * A2", 200.0);  // 10 * 20
    checkNumberResult("A2 / A1", 2.0);    // 20 / 10
    checkNumberResult("A1 ^ 2", 100.0);   // 10 ^ 2
}

TEST_F(EvaluatorTest, ArithmeticPrecedence) {
    checkNumberResult("1 + 2 * 3", 7.0);    // 1 + (2 * 3)
    checkNumberResult("2 * 3 + 4", 10.0);   // (2 * 3) + 4
    checkNumberResult("2 ^ 3 ^ 2", 512.0);  // 2 ^ (3 ^ 2) = 2 ^ 9
    checkNumberResult("(1 + 2) * 3", 9.0);  // (1 + 2) * 3
}

TEST_F(EvaluatorTest, UnaryOperators) {
    checkNumberResult("-5", -5.0);
    checkNumberResult("+10", 10.0);
    checkNumberResult("-A1", -10.0);
    checkNumberResult("-(A1 + A2)", -30.0);
}

TEST_F(EvaluatorTest, Comparison) {
    checkBooleanResult("1 = 1", true);
    checkBooleanResult("1 = 2", false);
    checkBooleanResult("1 <> 2", true);
    checkBooleanResult("1 <> 1", false);
    checkBooleanResult("1 < 2", true);
    checkBooleanResult("2 < 1", false);
    checkBooleanResult("1 <= 1", true);
    checkBooleanResult("1 <= 2", true);
    checkBooleanResult("2 <= 1", false);
    checkBooleanResult("2 > 1", true);
    checkBooleanResult("1 > 2", false);
    checkBooleanResult("2 >= 2", true);
    checkBooleanResult("2 >= 1", true);
    checkBooleanResult("1 >= 2", false);
}

TEST_F(EvaluatorTest, TextConcatenation) {
    checkTextResult("\"hello\" & \" world\"", "hello world");
    checkTextResult("B1 & \" \" & B2", "Hello World");
    checkTextResult("\"Value: \" & A1", "Value: 10");
}

TEST_F(EvaluatorTest, ArithmeticErrors) {
    checkErrorResult("1 / 0", ErrorType::DIV_ZERO);
    checkErrorResult("A1 / 0", ErrorType::DIV_ZERO);
    checkErrorResult("\"hello\" + 1", ErrorType::VALUE_ERROR);
    // Excel coerces booleans in arithmetic: TRUE*FALSE = 0
    checkNumberResult("TRUE * FALSE", 0.0);
}

TEST_F(EvaluatorTest, SumFunction) {
    checkNumberResult("SUM()", 0.0);
    checkNumberResult("SUM(1)", 1.0);
    checkNumberResult("SUM(1, 2, 3)", 6.0);
    checkNumberResult("SUM(A1, A2, A3)", 60.0);  // 10 + 20 + 30
    checkNumberResult("SUM(A1, 5, A2)", 35.0);   // 10 + 5 + 20
}

TEST_F(EvaluatorTest, MaxFunction) {
    checkNumberResult("MAX(1)", 1.0);
    checkNumberResult("MAX(1, 2, 3)", 3.0);
    checkNumberResult("MAX(A1, A2, A3)", 30.0);  // max(10, 20, 30)
    checkNumberResult("MAX(5, A1, 15)", 15.0);   // max(5, 10, 15)
}

TEST_F(EvaluatorTest, ConcatenateFunction) {
    checkTextResult("CONCATENATE()", "");
    checkTextResult("CONCATENATE(\"hello\")", "hello");
    checkTextResult("CONCATENATE(\"hello\", \" \", \"world\")", "hello world");
    checkTextResult("CONCATENATE(B1, \" \", B2)", "Hello World");
    checkTextResult("CONCATENATE(\"Value: \", A1)", "Value: 10");
}

TEST_F(EvaluatorTest, TrimFunction) {
    checkTextResult("TRIM(\"  hello  \")", "hello");
    checkTextResult("TRIM(\"hello\")", "hello");
    checkTextResult("TRIM(\"  hello world  \")", "hello world");
    checkTextResult("TRIM(\"\")", "");

    // Test with non-text values
    checkTextResult("TRIM(A1)", "10");
}

TEST_F(EvaluatorTest, BooleanFunctions) {
    checkBooleanResult("TRUE()", true);
    checkBooleanResult("FALSE()", false);
}

TEST_F(EvaluatorTest, IfFunction) {
    checkNumberResult("IF(TRUE, 1, 2)", 1.0);
    checkNumberResult("IF(FALSE, 1, 2)", 2.0);
    checkNumberResult("IF(A1 > 5, A1, 0)", 10.0);  // A1 = 10 > 5, so return A1
    checkNumberResult("IF(A1 > 50, A1, 0)", 0.0);  // A1 = 10 < 50, so return 0
    checkTextResult("IF(C1, B1, B2)", "Hello");    // C1 = true, so return B1
    checkTextResult("IF(C2, B1, B2)", "World");    // C2 = false, so return B2
}

TEST_F(EvaluatorTest, LenFunction) {
    checkNumberResult("LEN(\"hello\")", 5.0);
    checkNumberResult("LEN(\"\")", 0.0);
    checkNumberResult("LEN(B1)", 5.0);  // "Hello" has 5 characters
    checkNumberResult("LEN(A1)", 2.0);  // "10" has 2 characters
}

TEST_F(EvaluatorTest, AbsFunction) {
    checkNumberResult("ABS(5)", 5.0);
    checkNumberResult("ABS(-5)", 5.0);
    checkNumberResult("ABS(0)", 0.0);
    checkNumberResult("ABS(A1)", 10.0);
    checkNumberResult("ABS(-A1)", 10.0);
}

TEST_F(EvaluatorTest, RoundFunction) {
    checkNumberResult("ROUND(3.14159)", 3.0);
    checkNumberResult("ROUND(3.14159, 2)", 3.14);
    checkNumberResult("ROUND(3.14159, 4)", 3.1416);
    checkNumberResult("ROUND(1234.5, -1)", 1230.0);
    checkNumberResult("ROUND(1234.5, -2)", 1200.0);
}

TEST_F(EvaluatorTest, NestedFunctions) {
    // With A1=10, A2=20, A3=30; MAX(A1,A2)=20, MIN(A2,A3)=20; SUM=40
    checkNumberResult("SUM(MAX(A1, A2), MIN(A2, A3))", 40.0);
    checkNumberResult("ABS(SUM(-5, -10))", 15.0);  // ABS(-15) = 15
    checkTextResult("CONCATENATE(\"Sum: \", SUM(A1, A2))", "Sum: 30");
}

TEST_F(EvaluatorTest, ComplexExpressions) {
    checkNumberResult("(A1 + A2) * (A3 - A1) / 10", 60.0);  // (10+20) * (30-10) / 10 = 30 * 20 / 10
    checkBooleanResult("SUM(A1, A2) > A3", false);          // 30 > 30 is false
    checkBooleanResult("SUM(A1, A2) >= A3", true);          // 30 >= 30 is true
    checkTextResult("\"Result: \" & (A1 + A2)", "Result: 30");
}

TEST_F(EvaluatorTest, FunctionErrors) {
    // Non-existent function
    checkErrorResult("NONEXISTENT()", ErrorType::NAME_ERROR);

    // Wrong number of arguments
    checkErrorResult("TRIM()", ErrorType::VALUE_ERROR);
    checkErrorResult("TRIM(\"a\", \"b\")", ErrorType::VALUE_ERROR);

    // Type errors
    checkErrorResult("ABS(\"hello\")", ErrorType::VALUE_ERROR);
}

class FormulaEngineTest : public ::testing::Test {
  protected:
    FormulaEngine engine;

    void SetUp() override {
        engine.setVariable("A1", Value(10.0));
        engine.setVariable("A2", Value(20.0));
        engine.setVariable("text", Value("Hello"));
    }
};

TEST_F(FormulaEngineTest, BasicEvaluation) {
    auto result = engine.evaluate("A1 + A2");
    EXPECT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isNumber());
    EXPECT_DOUBLE_EQ(30.0, result.getValue().asNumber());
}

TEST_F(FormulaEngineTest, ParseError) {
    auto result = engine.evaluate("A1 +");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::PARSE_ERROR, result.getValue().asError());
}

TEST_F(FormulaEngineTest, VariableManagement) {
    EXPECT_TRUE(engine.getVariable("A1").isNumber());
    EXPECT_DOUBLE_EQ(10.0, engine.getVariable("A1").asNumber());

    engine.setVariable("A1", Value(50.0));
    EXPECT_DOUBLE_EQ(50.0, engine.getVariable("A1").asNumber());

    Value nonexistent = engine.getVariable("NONEXISTENT");
    EXPECT_TRUE(nonexistent.isEmpty());
}

TEST_F(FormulaEngineTest, CustomFunction) {
    // Register a simple DOUBLE function
    engine.registerFunction("DOUBLE",
                            [](const std::vector<Value>& args, const Context& ctx) -> Value {
                                (void)ctx;  // Unused
                                if (args.size() != 1)
                                    return Value::error(ErrorType::VALUE_ERROR);
                                if (!args[0].canConvertToNumber())
                                    return Value::error(ErrorType::VALUE_ERROR);
                                return Value(args[0].toNumber() * 2);
                            });

    auto result = engine.evaluate("DOUBLE(A1)");
    EXPECT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isNumber());
    EXPECT_DOUBLE_EQ(20.0, result.getValue().asNumber());  // 10 * 2
}

TEST_F(FormulaEngineTest, EvaluateWithOverrides_UsesOverridesAndFallsBack) {
    engine.setVariable("X", Value(5.0));
    // Y only provided via overrides; X should fall back to engine context
    std::unordered_map<std::string, Value> vars{{"Y", Value(3.0)}};
    auto res = engine.evaluate("X + Y", vars);
    ASSERT_TRUE(res.isSuccess());
    ASSERT_TRUE(res.getValue().isNumber());
    EXPECT_DOUBLE_EQ(8.0, res.getValue().asNumber());

    // Override X for this call only; engine's X should remain unchanged afterward
    std::unordered_map<std::string, Value> vars2{{"X", Value(10.0)}};
    auto res2 = engine.evaluate("X + 1", vars2);
    ASSERT_TRUE(res2.isSuccess());
    ASSERT_TRUE(res2.getValue().isNumber());
    EXPECT_DOUBLE_EQ(11.0, res2.getValue().asNumber());

    // Verify engine context restored
    auto res3 = engine.evaluate("X");
    ASSERT_TRUE(res3.isSuccess());
    ASSERT_TRUE(res3.getValue().isNumber());
    EXPECT_DOUBLE_EQ(5.0, res3.getValue().asNumber());
}