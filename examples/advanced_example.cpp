#include <xl-formula/xl-formula.h>
#include <iomanip>
#include <iostream>
#include <map>

using namespace xl_formula;

// Custom function example: FACTORIAL
Value factorial(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    if (!arg.canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    double n = arg.toNumber();
    if (n < 0 || n != static_cast<int>(n) || n > 20) {  // Limit to reasonable range
        return Value::error(ErrorType::NUM_ERROR);
    }

    double result = 1.0;
    for (int i = 2; i <= static_cast<int>(n); ++i) {
        result *= i;
    }

    return Value(result);
}

// Custom function example: FIBONACCI
Value fibonacci(const std::vector<Value>& args, const Context& context) {
    (void)context;  // Unused parameter

    if (args.size() != 1) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    const auto& arg = args[0];
    if (arg.isError()) {
        return arg;
    }

    if (!arg.canConvertToNumber()) {
        return Value::error(ErrorType::VALUE_ERROR);
    }

    int n = static_cast<int>(arg.toNumber());
    if (n < 0 || n > 40) {  // Limit to reasonable range
        return Value::error(ErrorType::NUM_ERROR);
    }

    if (n <= 1)
        return Value(static_cast<double>(n));

    double a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        double temp = a + b;
        a = b;
        b = temp;
    }

    return Value(b);
}

// Simulate a simple spreadsheet
class SimpleSpreadsheet {
  private:
    FormulaEngine engine_;
    std::map<std::string, std::string> formulas_;

  public:
    SimpleSpreadsheet() {
        // Register custom functions
        engine_.registerFunction("FACTORIAL", factorial);
        engine_.registerFunction("FIBONACCI", fibonacci);

        // Add some utility functions
        engine_.registerFunction("AVERAGE",
                                 [](const std::vector<Value>& args, const Context& ctx) -> Value {
                                     (void)ctx;
                                     if (args.empty())
                                         return Value(0.0);

                                     Value sum_result = functions::builtin::sum(args, ctx);
                                     if (sum_result.isError())
                                         return sum_result;

                                     return Value(sum_result.asNumber() / args.size());
                                 });

        engine_.registerFunction("MIN",
                                 [](const std::vector<Value>& args, const Context& ctx) -> Value {
                                     (void)ctx;
                                     if (args.empty())
                                         return Value::error(ErrorType::VALUE_ERROR);

                                     Value min_val = args[0];
                                     for (const auto& arg : args) {
                                         if (arg.isError())
                                             return arg;
                                         if (arg < min_val)
                                             min_val = arg;
                                     }
                                     return min_val;
                                 });
    }

    void setCell(const std::string& cell, const Value& value) {
        engine_.setVariable(cell, value);
        formulas_.erase(cell);  // Clear any formula for this cell
    }

    void setFormula(const std::string& cell, const std::string& formula) {
        formulas_[cell] = formula;
        auto result = engine_.evaluate(formula);
        if (result.isSuccess()) {
            engine_.setVariable(cell, result.getValue());
        } else {
            engine_.setVariable(cell, result.getValue());  // Store error
        }
    }

    Value getCell(const std::string& cell) {
        return engine_.getVariable(cell);
    }

    void printCell(const std::string& cell) {
        auto value = getCell(cell);
        std::cout << std::setw(8) << cell << ": ";

        if (formulas_.find(cell) != formulas_.end()) {
            std::cout << formulas_[cell] << " => ";
        }

        std::cout << value.toString() << std::endl;
    }

    void recalculate() {
        // Recalculate all formulas
        for (const auto& [cell, formula] : formulas_) {
            auto result = engine_.evaluate(formula);
            if (result.isSuccess()) {
                engine_.setVariable(cell, result.getValue());
            } else {
                engine_.setVariable(cell, result.getValue());  // Store error
            }
        }
    }
};

int main() {
    std::cout << "=== XL-Formula Advanced Example ===" << std::endl;
    std::cout << "Simulating a Spreadsheet with Custom Functions" << std::endl << std::endl;

    SimpleSpreadsheet sheet;

    // Set up some data
    std::cout << "Setting up spreadsheet data:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    // Sales data
    sheet.setCell("A1", Value("Product"));
    sheet.setCell("A2", Value("Laptop"));
    sheet.setCell("A3", Value("Mouse"));
    sheet.setCell("A4", Value("Keyboard"));
    sheet.setCell("A5", Value("Monitor"));

    sheet.setCell("B1", Value("Price"));
    sheet.setCell("B2", Value(1200.0));
    sheet.setCell("B3", Value(25.0));
    sheet.setCell("B4", Value(75.0));
    sheet.setCell("B5", Value(300.0));

    sheet.setCell("C1", Value("Quantity"));
    sheet.setCell("C2", Value(10.0));
    sheet.setCell("C3", Value(50.0));
    sheet.setCell("C4", Value(30.0));
    sheet.setCell("C5", Value(15.0));

    // Calculate totals using formulas
    sheet.setFormula("D1", "\"Total\"");
    sheet.setFormula("D2", "B2 * C2");
    sheet.setFormula("D3", "B3 * C3");
    sheet.setFormula("D4", "B4 * C4");
    sheet.setFormula("D5", "B5 * C5");

    // Summary calculations
    sheet.setFormula("E1", "\"Summary\"");
    sheet.setFormula("E2", "SUM(D2, D3, D4, D5)");
    sheet.setFormula("E3", "AVERAGE(B2, B3, B4, B5)");
    sheet.setFormula("E4", "MAX(D2, D3, D4, D5)");
    sheet.setFormula("E5", "MIN(D2, D3, D4, D5)");

    // Print the "spreadsheet"
    std::cout << "Spreadsheet contents:" << std::endl;
    for (char col = 'A'; col <= 'E'; ++col) {
        for (int row = 1; row <= 5; ++row) {
            std::string cell = std::string(1, col) + std::to_string(row);
            sheet.printCell(cell);
        }
        std::cout << std::endl;
    }

    // Demonstrate custom functions
    std::cout << "Custom function examples:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    sheet.setFormula("F1", "FACTORIAL(5)");
    sheet.setFormula("F2", "FIBONACCI(10)");
    sheet.setFormula("F3", "FACTORIAL(0)");
    sheet.setFormula("F4", "FIBONACCI(0)");
    sheet.setFormula("F5", "FACTORIAL(C2 / 2)");  // Should be FACTORIAL(5)

    sheet.printCell("F1");
    sheet.printCell("F2");
    sheet.printCell("F3");
    sheet.printCell("F4");
    sheet.printCell("F5");
    std::cout << std::endl;

    // Complex expressions
    std::cout << "Complex business logic:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    // Tax and discount calculations
    sheet.setCell("tax_rate", Value(0.08));
    sheet.setCell("discount_threshold", Value(10000.0));
    sheet.setCell("discount_rate", Value(0.05));

    sheet.setFormula("G1", "\"Tax\"");
    sheet.setFormula("G2", "E2 * tax_rate");

    sheet.setFormula("H1", "\"Discount\"");
    sheet.setFormula("H2", "IF(E2 > discount_threshold, E2 * discount_rate, 0)");

    sheet.setFormula("I1", "\"Final Total\"");
    sheet.setFormula("I2", "E2 + G2 - H2");

    sheet.printCell("G1");
    sheet.printCell("G2");
    sheet.printCell("H1");
    sheet.printCell("H2");
    sheet.printCell("I1");
    sheet.printCell("I2");
    std::cout << std::endl;

    // Demonstrate formula dependency and recalculation
    std::cout << "Updating data and recalculating:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    std::cout << "Original laptop quantity: " << sheet.getCell("C2").toString() << std::endl;
    std::cout << "Original total: " << sheet.getCell("E2").toString() << std::endl;
    std::cout << "Original final total: " << sheet.getCell("I2").toString() << std::endl;

    // Update laptop quantity
    sheet.setCell("C2", Value(20.0));  // Double the quantity
    sheet.recalculate();

    std::cout << std::endl << "After doubling laptop quantity:" << std::endl;
    std::cout << "New laptop quantity: " << sheet.getCell("C2").toString() << std::endl;
    std::cout << "New total: " << sheet.getCell("E2").toString() << std::endl;
    std::cout << "New final total: " << sheet.getCell("I2").toString() << std::endl;
    std::cout << "Discount applied: " << sheet.getCell("H2").toString() << std::endl;
    std::cout << std::endl;

    // Text manipulation examples
    std::cout << "Text manipulation examples:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    sheet.setFormula("J1", "CONCATENATE(\"Top product: \", A2)");
    sheet.setFormula("J2", "\"Report: \" & A2 & \" sales = \" & D2");
    sheet.setFormula("J3", "TRIM(\"  Formatted text  \")");
    sheet.setFormula("J4", "\"Length of A2: \" & LEN(A2)");
    sheet.setFormula("J5", "IF(LEN(A2) > 5, \"Long name\", \"Short name\")");

    sheet.printCell("J1");
    sheet.printCell("J2");
    sheet.printCell("J3");
    sheet.printCell("J4");
    sheet.printCell("J5");
    std::cout << std::endl;

    // Error handling demonstration
    std::cout << "Error handling examples:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    sheet.setFormula("K1", "B2 / 0");                 // Division by zero
    sheet.setFormula("K2", "UNKNOWN_FUNCTION(B2)");   // Unknown function
    sheet.setFormula("K3", "B2 + UNKNOWN_VAR");       // Unknown variable
    sheet.setFormula("K4", "FACTORIAL(-1)");          // Invalid argument
    sheet.setFormula("K5", "ABS(\"not a number\")");  // Type error

    sheet.printCell("K1");
    sheet.printCell("K2");
    sheet.printCell("K3");
    sheet.printCell("K4");
    sheet.printCell("K5");

    std::cout << std::endl << "=== Advanced Example Complete ===" << std::endl;

    return 0;
}