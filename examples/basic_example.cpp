#include <xl-formula/xl-formula.h>
#include <iostream>

using namespace xl_formula;

int main() {
    std::cout << "=== XL-Formula Basic Example ===" << std::endl;
    std::cout << "Library Version: " << Version::toString() << std::endl << std::endl;

    // Create a formula engine
    FormulaEngine engine;

    // Set some variables (like spreadsheet cells)
    engine.setVariable("A1", Value(10.0));
    engine.setVariable("A2", Value(20.0));
    engine.setVariable("A3", Value(30.0));
    engine.setVariable("name", Value("Excel"));
    engine.setVariable("version", Value(2024.0));

    // List of formulas to test
    std::vector<std::pair<std::string, std::string>> formulas = {
            {"Simple arithmetic", "A1 + A2"},
            {"Function call", "SUM(A1, A2, A3)"},
            {"Complex expression", "(A1 + A2) * A3 / 2"},
            {"Text concatenation", "\"Hello \" & name & \"!\""},
            {"Conditional logic", "IF(A1 > 5, \"High\", \"Low\")"},
            {"Nested functions", "ROUND(SUM(A1, A2, A3) / 3, 2)"},
            {"Boolean operations", "A1 < A2"},
            {"Text functions", "LEN(name)"},
            {"Math functions", "ABS(A1 - A2 - A3)"},
            {"Mixed types", "\"Version: \" & version"}};

    std::cout << "Evaluating formulas:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    for (const auto& [description, formula] : formulas) {
        std::cout << description << ":" << std::endl;
        std::cout << "  Formula: " << formula << std::endl;

        auto result = engine.evaluate(formula);
        if (result.isSuccess()) {
            std::cout << "  Result:  " << result.getValue().toString() << std::endl;
        } else {
            std::cout << "  Error:   " << result.getValue().toString() << std::endl;
        }
        std::cout << std::endl;
    }

    // Demonstrate error handling
    std::cout << "Error handling examples:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    std::vector<std::pair<std::string, std::string>> error_cases = {
            {"Division by zero", "A1 / 0"},
            {"Unknown variable", "UNKNOWN_VAR + A1"},
            {"Unknown function", "UNKNOWN_FUNC(A1)"},
            {"Parse error", "A1 +"},
            {"Type error", "ABS(\"hello\")"}};

    for (const auto& [description, formula] : error_cases) {
        std::cout << description << ":" << std::endl;
        std::cout << "  Formula: " << formula << std::endl;

        auto result = engine.evaluate(formula);
        std::cout << "  Error:   " << result.getValue().toString() << std::endl;
        std::cout << std::endl;
    }

    // Demonstrate variable management
    std::cout << "Variable management:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    std::cout << "Current variables in context:" << std::endl;
    for (const auto& var_name : engine.getContext().getVariableNames()) {
        auto value = engine.getVariable(var_name);
        std::cout << "  " << var_name << " = " << value.toString() << std::endl;
    }

    std::cout << std::endl
              << "Updating A1 from " << engine.getVariable("A1").toString() << " to 100..."
              << std::endl;
    engine.setVariable("A1", Value(100.0));

    auto result = engine.evaluate("SUM(A1, A2, A3)");
    std::cout << "SUM(A1, A2, A3) = " << result.getValue().toString() << std::endl;

    std::cout << std::endl << "=== Example Complete ===" << std::endl;

    return 0;
}