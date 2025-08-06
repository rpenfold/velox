#pragma once

/**
 * @file xl-formula.h
 * @brief Main header for the XL-Formula library
 *
 * This file provides the main API for the XL-Formula library, a high-performance
 * Excel-like formula parsing and evaluation engine implemented in C++.
 *
 * ## Basic Usage
 *
 * ```cpp
 * #include <xl-formula/xl-formula.h>
 *
 * // Create a formula engine
 * xl_formula::FormulaEngine engine;
 *
 * // Set some variables
 * engine.setVariable("A1", xl_formula::Value(10.0));
 * engine.setVariable("A2", xl_formula::Value(20.0));
 *
 * // Evaluate a formula
 * auto result = engine.evaluate("SUM(A1, A2, 5)");
 * if (result.isSuccess()) {
 *     std::cout << "Result: " << result.getValue().toString() << std::endl; // "35"
 * }
 * ```
 *
 * ## Supported Features
 *
 * ### Data Types
 * - Numbers (double precision)
 * - Text/strings
 * - Booleans (TRUE/FALSE)
 * - Dates
 * - Errors (#DIV/0!, #VALUE!, etc.)
 * - Empty values
 *
 * ### Operations
 * - Arithmetic: +, -, *, /, ^
 * - Comparison: =, <>, <, <=, >, >=
 * - Text concatenation: &
 * - Unary operators: +, -
 *
 * ### Built-in Functions
 * - **Math**: SUM, MAX, ABS, ROUND
 * - **Text**: CONCATENATE, TRIM, LEN
 * - **Logical**: TRUE, FALSE, IF
 *
 * ### Context Variables
 * Variables can be set in the evaluation context and referenced in formulas.
 * Variable names are case-sensitive.
 *
 * ## Advanced Usage
 *
 * ### Custom Functions
 *
 * ```cpp
 * engine.registerFunction("DOUBLE", [](const std::vector<xl_formula::Value>& args, const
 * xl_formula::Context& ctx) { if (args.size() != 1) return
 * xl_formula::Value::error(xl_formula::ErrorType::VALUE_ERROR); if (!args[0].canConvertToNumber())
 * return xl_formula::Value::error(xl_formula::ErrorType::VALUE_ERROR); return
 * xl_formula::Value(args[0].toNumber() * 2);
 * });
 * ```
 *
 * ### Error Handling
 *
 * The library provides comprehensive error handling with specific error types:
 * - Parse errors for invalid syntax
 * - Runtime errors for invalid operations
 * - Type conversion errors
 * - Function-specific errors
 *
 * @version 1.0.0
 * @author XL-Formula Team
 */

// Core types and values
#include "types.h"

// AST and parsing
#include "ast.h"
#include "parser.h"

// Evaluation engine
#include "evaluator.h"

// Built-in functions
#include "functions.h"

namespace xl_formula {

/**
 * @brief Library version information
 */
struct Version {
    static constexpr int MAJOR = 1;
    static constexpr int MINOR = 0;
    static constexpr int PATCH = 0;

    /**
     * @brief Get version string
     * @return Version string in format "major.minor.patch"
     */
    static std::string toString() {
        return std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH);
    }
};

/**
 * @brief Quick evaluation function for simple use cases
 * @param formula Formula string to evaluate
 * @param context Optional context with variables (default: empty)
 * @return Evaluation result
 *
 * This is a convenience function for one-off evaluations. For repeated
 * evaluations or when you need to maintain state, use FormulaEngine directly.
 *
 * Example:
 * ```cpp
 * auto result = xl_formula::evaluate("2 + 3 * 4");
 * // result.getValue().asNumber() == 14.0
 * ```
 */
EvaluationResult evaluate(const std::string& formula, const Context& context = Context());

/**
 * @brief Parse formula into AST without evaluation
 * @param formula Formula string to parse
 * @return Parse result containing AST or errors
 *
 * This function allows you to parse formulas into an Abstract Syntax Tree
 * without evaluating them. Useful for analysis, validation, or custom evaluation.
 *
 * Example:
 * ```cpp
 * auto parse_result = xl_formula::parse("SUM(A1:A10)");
 * if (parse_result.isSuccess()) {
 *     std::cout << "AST: " << parse_result.getAST()->toString() << std::endl;
 * }
 * ```
 */
ParseResult parse(const std::string& formula);

}  // namespace xl_formula