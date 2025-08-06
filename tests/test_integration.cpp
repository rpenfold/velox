#include <gtest/gtest.h>
#include <xl-formula/xl-formula.h>

using namespace xl_formula;

class IntegrationTest : public ::testing::Test {
  protected:
    FormulaEngine engine;

    void SetUp() override {
        // Set up a sample spreadsheet-like context
        engine.setVariable("A1", Value(10.0));
        engine.setVariable("A2", Value(20.0));
        engine.setVariable("A3", Value(30.0));
        engine.setVariable("A4", Value(40.0));
        engine.setVariable("A5", Value(50.0));

        engine.setVariable("B1", Value("Product"));
        engine.setVariable("B2", Value("Sales"));
        engine.setVariable("B3", Value("Profit"));

        engine.setVariable("C1", Value("Apple"));
        engine.setVariable("C2", Value("Banana"));
        engine.setVariable("C3", Value("Cherry"));

        engine.setVariable("tax_rate", Value(0.1));
        engine.setVariable("discount", Value(0.05));
    }

    void checkFormulaResult(const std::string& formula, const Value& expected) {
        auto result = engine.evaluate(formula);
        EXPECT_TRUE(result.isSuccess()) << "Formula failed: " << formula;
        if (result.isSuccess()) {
            if (expected.isNumber()) {
                EXPECT_TRUE(result.getValue().isNumber()) << "Expected number for: " << formula;
                if (result.getValue().isNumber()) {
                    EXPECT_DOUBLE_EQ(expected.asNumber(), result.getValue().asNumber())
                            << "Formula: " << formula;
                }
            } else if (expected.isText()) {
                EXPECT_TRUE(result.getValue().isText()) << "Expected text for: " << formula;
                if (result.getValue().isText()) {
                    EXPECT_EQ(expected.asText(), result.getValue().asText())
                            << "Formula: " << formula;
                }
            } else if (expected.isBoolean()) {
                EXPECT_TRUE(result.getValue().isBoolean()) << "Expected boolean for: " << formula;
                if (result.getValue().isBoolean()) {
                    EXPECT_EQ(expected.asBoolean(), result.getValue().asBoolean())
                            << "Formula: " << formula;
                }
            }
        }
    }
};

TEST_F(IntegrationTest, BasicSpreadsheetFormulas) {
    // Simple sum
    checkFormulaResult("SUM(A1, A2, A3)", Value(60.0));

    // Average calculation
    checkFormulaResult("SUM(A1, A2, A3, A4, A5) / 5", Value(30.0));

    // Percentage calculation
    checkFormulaResult("A1 / SUM(A1, A2, A3, A4, A5) * 100", Value(6.666667));  // Close enough

    // Tax calculation
    checkFormulaResult("A1 * (1 + tax_rate)", Value(11.0));

    // Discount calculation
    checkFormulaResult("A2 * (1 - discount)", Value(19.0));
}

TEST_F(IntegrationTest, TextManipulation) {
    // Concatenation
    checkFormulaResult("B1 & \": \" & C1", Value("Product: Apple"));

    // Multi-part concatenation
    checkFormulaResult("CONCATENATE(B1, \" \", C1, \" - \", A1)", Value("Product Apple - 10"));

    // Text with numbers
    checkFormulaResult("\"Total: \" & SUM(A1, A2)", Value("Total: 30"));

    // Trimming
    engine.setVariable("dirty_text", Value("  Hello World  "));
    checkFormulaResult("TRIM(dirty_text)", Value("Hello World"));

    // Length calculation
    checkFormulaResult("LEN(C1)", Value(5.0));  // "Apple" = 5 chars
}

TEST_F(IntegrationTest, ConditionalLogic) {
    // Basic IF statements
    checkFormulaResult("IF(A1 > 5, \"High\", \"Low\")", Value("High"));
    checkFormulaResult("IF(A1 > 50, \"High\", \"Low\")", Value("Low"));

    // Nested conditions
    checkFormulaResult("IF(A1 > 50, \"High\", IF(A1 > 10, \"Medium\", \"Low\"))", Value("Low"));

    // Conditional calculations
    checkFormulaResult("IF(A1 > A2, A1 - A2, A2 - A1)", Value(10.0));  // |10 - 20| = 10

    // Boolean operations with numbers
    checkFormulaResult("IF(SUM(A1, A2) = 30, TRUE(), FALSE())", Value(true));
}

TEST_F(IntegrationTest, MathematicalFormulas) {
    // Power calculations
    checkFormulaResult("A1 ^ 2", Value(100.0));

    // Complex expression
    checkFormulaResult("(A1 + A2) * A3 / (A4 - A5)",
                       Value(-90.0));  // (10+20)*30/(40-50) = 30*30/(-10)

    // Absolute values
    checkFormulaResult("ABS(A4 - A5)", Value(10.0));  // |40 - 50| = 10

    // Rounding
    engine.setVariable("pi", Value(3.14159));
    checkFormulaResult("ROUND(pi, 2)", Value(3.14));

    // Maximum values
    checkFormulaResult("MAX(A1, A2, A3, A4, A5)", Value(50.0));
}

TEST_F(IntegrationTest, FinancialCalculations) {
    // Simple interest
    engine.setVariable("principal", Value(1000.0));
    engine.setVariable("rate", Value(0.05));
    engine.setVariable("time", Value(2.0));

    checkFormulaResult("principal * rate * time", Value(100.0));

    // Compound interest (simplified)
    checkFormulaResult("principal * ((1 + rate) ^ time)", Value(1102.5));

    // Profit margin
    engine.setVariable("revenue", Value(1000.0));
    engine.setVariable("cost", Value(750.0));

    checkFormulaResult("(revenue - cost) / revenue * 100", Value(25.0));
}

TEST_F(IntegrationTest, DataValidation) {
    // Check for valid ranges
    checkFormulaResult("IF(A1 >= 0, IF(A1 <= 100, \"Valid\", \"Too High\"), \"Negative\")",
                       Value("Valid"));

    // Text validation
    checkFormulaResult("IF(LEN(C1) > 0, \"Has Value\", \"Empty\")", Value("Has Value"));

    // Conditional formatting logic
    checkFormulaResult("IF(A1 > MAX(A2, A3), \"Highlight\", \"Normal\")", Value("Normal"));
}

TEST_F(IntegrationTest, ComplexNestedFormulas) {
    // Multi-level nesting
    checkFormulaResult(
            "IF(SUM(A1, A2) > 25, "
            "   CONCATENATE(\"High: \", MAX(A1, A2)), "
            "   CONCATENATE(\"Low: \", ABS(A1 - A2)))",
            Value("High: 20"));

    // Mathematical expression with functions
    checkFormulaResult("ROUND(SUM(A1, A2, A3) / 3, 1)",
                       Value(20.0)  // (10+20+30)/3 = 20.0
    );

    // Text and number combination
    checkFormulaResult("CONCATENATE(B1, \": \", IF(A1 > 0, A1, \"N/A\"))", Value("Product: 10"));
}

TEST_F(IntegrationTest, ErrorHandling) {
    // Division by zero
    auto result = engine.evaluate("A1 / 0");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::DIV_ZERO, result.getValue().asError());

    // Invalid function
    result = engine.evaluate("INVALID_FUNCTION(A1)");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::NAME_ERROR, result.getValue().asError());

    // Invalid variable
    result = engine.evaluate("NONEXISTENT_VAR + A1");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::NAME_ERROR, result.getValue().asError());

    // Parse error
    result = engine.evaluate("A1 +");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::PARSE_ERROR, result.getValue().asError());

    // Type error
    result = engine.evaluate("ABS(\"hello\")");
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isError());
    EXPECT_EQ(ErrorType::VALUE_ERROR, result.getValue().asError());
}

TEST_F(IntegrationTest, CustomFunctionIntegration) {
    // Register a custom function for percentage calculation
    engine.registerFunction(
            "PERCENT", [](const std::vector<Value>& args, const Context& ctx) -> Value {
                (void)ctx;  // Unused
                if (args.size() != 2)
                    return Value::error(ErrorType::VALUE_ERROR);

                if (!args[0].canConvertToNumber() || !args[1].canConvertToNumber()) {
                    return Value::error(ErrorType::VALUE_ERROR);
                }

                double numerator = args[0].toNumber();
                double denominator = args[1].toNumber();

                if (denominator == 0.0) {
                    return Value::error(ErrorType::DIV_ZERO);
                }

                return Value((numerator / denominator) * 100.0);
            });

    // Test custom function
    checkFormulaResult("PERCENT(A1, SUM(A1, A2, A3))", Value(16.666667));  // Close enough

    // Custom function in complex expression
    checkFormulaResult("\"A1 is \" & ROUND(PERCENT(A1, SUM(A1, A2, A3)), 1) & \"% of total\"",
                       Value("A1 is 16.7% of total"));
}

// Test the convenience functions
TEST_F(IntegrationTest, ConvenienceFunctions) {
    Context ctx;
    ctx.setVariable("X", Value(5.0));
    ctx.setVariable("Y", Value(3.0));

    // Test standalone evaluate function
    auto result = xl_formula::evaluate("X + Y", ctx);
    EXPECT_TRUE(result.isSuccess());
    EXPECT_TRUE(result.getValue().isNumber());
    EXPECT_DOUBLE_EQ(8.0, result.getValue().asNumber());

    // Test standalone parse function
    auto parse_result = xl_formula::parse("SUM(1, 2, 3)");
    EXPECT_TRUE(parse_result.isSuccess());
    EXPECT_NE(nullptr, parse_result.getAST());

    // Parse error
    parse_result = xl_formula::parse("1 +");
    EXPECT_FALSE(parse_result.isSuccess());
    EXPECT_TRUE(parse_result.hasErrors());
}

TEST_F(IntegrationTest, PerformanceBaseline) {
    // Simple performance test - not a benchmark, just ensures reasonable performance
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        auto result = engine.evaluate("SUM(A1, A2, A3) * 2 + MAX(A4, A5)");
        EXPECT_TRUE(result.isSuccess());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Should complete 1000 evaluations in reasonable time (adjust threshold as needed)
    EXPECT_LT(duration.count(), 1000) << "Performance regression detected";
}