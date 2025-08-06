#pragma once

#include <vector>
#include <stdexcept>
#include "evaluator.h"
#include "types.h"

namespace xl_formula {
namespace functions {

/**
 * @brief Built-in function implementations
 */
namespace builtin {

/**
 * @brief SUM function - adds all numeric arguments
 * @param args Function arguments
 * @param context Evaluation context (unused for SUM)
 * @return Sum of all numeric arguments
 */
Value sum(const std::vector<Value>& args, const Context& context);

/**
 * @brief MAX function - returns maximum value from arguments
 * Supports numbers, dates, and comparable types
 * @param args Function arguments
 * @param context Evaluation context (unused for MAX)
 * @return Maximum value from arguments
 */
Value max(const std::vector<Value>& args, const Context& context);

/**
 * @brief CONCATENATE function - concatenates all arguments as text
 * @param args Function arguments
 * @param context Evaluation context (unused for CONCATENATE)
 * @return Concatenated string
 */
Value concatenate(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRIM function - removes leading and trailing whitespace
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for TRIM)
 * @return Trimmed string
 */
Value trim(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRUE function - returns boolean true
 * @param args Function arguments (ignored)
 * @param context Evaluation context (unused for TRUE)
 * @return Boolean true value
 */
Value true_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief FALSE function - returns boolean false
 * @param args Function arguments (ignored)
 * @param context Evaluation context (unused for FALSE)
 * @return Boolean false value
 */
Value false_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief IF function - conditional logic
 * @param args Function arguments (condition, true_value, false_value)
 * @param context Evaluation context (unused for IF)
 * @return true_value if condition is true, false_value otherwise
 */
Value if_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LEN function - returns length of text
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for LEN)
 * @return Length of text as number
 */
Value len(const std::vector<Value>& args, const Context& context);

/**
 * @brief LEFT function - returns the leftmost characters from text
 * @param args Function arguments (text, [num_chars])
 * @param context Evaluation context (unused for LEFT)
 * @return Leftmost characters from text
 */
Value left(const std::vector<Value>& args, const Context& context);

/**
 * @brief RIGHT function - returns the rightmost characters from text
 * @param args Function arguments (text, [num_chars])
 * @param context Evaluation context (unused for RIGHT)
 * @return Rightmost characters from text
 */
Value right(const std::vector<Value>& args, const Context& context);

/**
 * @brief MID function - returns characters from the middle of text
 * @param args Function arguments (text, start_num, num_chars)
 * @param context Evaluation context (unused for MID)
 * @return Characters from the middle of text
 */
Value mid(const std::vector<Value>& args, const Context& context);

/**
 * @brief UPPER function - converts text to uppercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for UPPER)
 * @return Text converted to uppercase
 */
Value upper(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOWER function - converts text to lowercase
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for LOWER)
 * @return Text converted to lowercase
 */
Value lower(const std::vector<Value>& args, const Context& context);

/**
 * @brief PROPER function - capitalizes the first letter of each word
 * @param args Function arguments (expects 1 text argument)
 * @param context Evaluation context (unused for PROPER)
 * @return Text with first letter of each word capitalized
 */
Value proper(const std::vector<Value>& args, const Context& context);

/**
 * @brief ABS function - returns absolute value
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for ABS)
 * @return Absolute value
 */
Value abs_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ROUND function - rounds number to specified decimal places
 * @param args Function arguments (number, decimal_places)
 * @param context Evaluation context (unused for ROUND)
 * @return Rounded number
 */
Value round_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief MIN function - returns minimum value in a set of values
 * @param args Function arguments (values to compare)
 * @param context Evaluation context (unused for MIN)
 * @return Minimum value
 */
Value min(const std::vector<Value>& args, const Context& context);

/**
 * @brief AVERAGE function - returns average of arguments
 * @param args Function arguments (numeric values)
 * @param context Evaluation context (unused for AVERAGE)
 * @return Average value
 */
Value average(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNT function - counts numeric values
 * @param args Function arguments (values to count)
 * @param context Evaluation context (unused for COUNT)
 * @return Count of numeric values
 */
Value count(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNTA function - counts non-empty values
 * @param args Function arguments (values to count)
 * @param context Evaluation context (unused for COUNTA)
 * @return Count of non-empty values
 */
Value counta(const std::vector<Value>& args, const Context& context);

/**
 * @brief SQRT function - returns square root
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SQRT)
 * @return Square root
 */
Value sqrt_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief POWER function - returns number raised to a power
 * @param args Function arguments (base, exponent)
 * @param context Evaluation context (unused for POWER)
 * @return Base raised to exponent
 */
Value power(const std::vector<Value>& args, const Context& context);

/**
 * @brief MOD function - returns remainder from division
 * @param args Function arguments (numerator, divisor)
 * @param context Evaluation context (unused for MOD)
 * @return Remainder
 */
Value mod(const std::vector<Value>& args, const Context& context);

/**
 * @brief PI function - returns the value of pi
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for PI)
 * @return Value of pi
 */
Value pi(const std::vector<Value>& args, const Context& context);

/**
 * @brief SIGN function - returns the sign of a number
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SIGN)
 * @return 1 if positive, -1 if negative, 0 if zero
 */
Value sign(const std::vector<Value>& args, const Context& context);

/**
 * @brief INT function - rounds a number down to the nearest integer
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for INT)
 * @return Integer part of the number (rounded down)
 */
Value int_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TRUNC function - truncates a number to an integer
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for TRUNC)
 * @return Truncated number
 */
Value trunc_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief CEILING function - rounds a number up to the nearest multiple
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for CEILING)
 * @return Number rounded up to nearest multiple
 */
Value ceiling_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief FLOOR function - rounds a number down to the nearest multiple
 * @param args Function arguments (expects 1 or 2 arguments)
 * @param context Evaluation context (unused for FLOOR)
 * @return Number rounded down to nearest multiple
 */
Value floor_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RAND function - returns a random number between 0 and 1
 * @param args Function arguments (should be empty)
 * @param context Evaluation context (unused for RAND)
 * @return Random number between 0 and 1
 */
Value rand_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RANDBETWEEN function - returns a random number between specified numbers
 * @param args Function arguments (expects 2 numeric arguments)
 * @param context Evaluation context (unused for RANDBETWEEN)
 * @return Random integer between bottom and top (inclusive)
 */
Value randbetween(const std::vector<Value>& args, const Context& context);

/**
 * @brief COUNTIF function - counts the number of cells that meet a criterion
 * @param args Function arguments (expects 2+ arguments: range..., criteria)
 * @param context Evaluation context (unused for COUNTIF)
 * @return Count of values that match criteria
 */
Value countif(const std::vector<Value>& args, const Context& context);

/**
 * @brief MEDIAN function - returns the median of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for MEDIAN)
 * @return Median value of the arguments
 */
Value median(const std::vector<Value>& args, const Context& context);

/**
 * @brief MODE function - returns the most frequently occurring value
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for MODE)
 * @return Most frequently occurring value
 */
Value mode(const std::vector<Value>& args, const Context& context);

/**
 * @brief STDEV function - returns the standard deviation of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for STDEV)
 * @return Standard deviation of the arguments
 */
Value stdev(const std::vector<Value>& args, const Context& context);

/**
 * @brief VAR function - returns the variance of a set of numbers
 * @param args Function arguments (expects 1+ numeric arguments)
 * @param context Evaluation context (unused for VAR)
 * @return Variance of the arguments
 */
Value var(const std::vector<Value>& args, const Context& context);

/**
 * @brief FIND function - finds one text string within another (case-sensitive)
 * @param args Function arguments (find_text, within_text, [start_num])
 * @param context Evaluation context (unused for FIND)
 * @return Position of find_text within within_text, or #VALUE! if not found
 */
Value find(const std::vector<Value>& args, const Context& context);

/**
 * @brief SEARCH function - finds one text string within another (case-insensitive)
 * @param args Function arguments (find_text, within_text, [start_num])
 * @param context Evaluation context (unused for SEARCH)
 * @return Position of find_text within within_text, or #VALUE! if not found
 */
Value search(const std::vector<Value>& args, const Context& context);

/**
 * @brief REPLACE function - replaces part of a text string
 * @param args Function arguments (old_text, start_num, num_chars, new_text)
 * @param context Evaluation context (unused for REPLACE)
 * @return Text with the specified characters replaced
 */
Value replace(const std::vector<Value>& args, const Context& context);

/**
 * @brief SUBSTITUTE function - substitutes new text for old text
 * @param args Function arguments (text, old_text, new_text, [instance_num])
 * @param context Evaluation context (unused for SUBSTITUTE)
 * @return Text with old_text replaced by new_text
 */
Value substitute(const std::vector<Value>& args, const Context& context);

/**
 * @brief TEXT function - formats a number as text
 * @param args Function arguments (value, format_text)
 * @param context Evaluation context (unused for TEXT)
 * @return Formatted text string
 */
Value text(const std::vector<Value>& args, const Context& context);

/**
 * @brief VALUE function - converts a text string to a number
 * @param args Function arguments (text)
 * @param context Evaluation context (unused for VALUE)
 * @return Numeric value
 */
Value value(const std::vector<Value>& args, const Context& context);

/**
 * @brief SIN function - returns the sine of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for SIN)
 * @return Sine of the angle
 */
Value sin_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief COS function - returns the cosine of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for COS)
 * @return Cosine of the angle
 */
Value cos_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TAN function - returns the tangent of an angle
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for TAN)
 * @return Tangent of the angle
 */
Value tan_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ASIN function - returns the arcsine
 * @param args Function arguments (expects 1 numeric argument between -1 and 1)
 * @param context Evaluation context (unused for ASIN)
 * @return Arcsine in radians
 */
Value asin_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ACOS function - returns the arccosine
 * @param args Function arguments (expects 1 numeric argument between -1 and 1)
 * @param context Evaluation context (unused for ACOS)
 * @return Arccosine in radians
 */
Value acos_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ATAN function - returns the arctangent
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for ATAN)
 * @return Arctangent in radians
 */
Value atan_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief ATAN2 function - returns the arctangent from x and y coordinates
 * @param args Function arguments (expects 2 numeric arguments: y, x)
 * @param context Evaluation context (unused for ATAN2)
 * @return Arctangent in radians
 */
Value atan2_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief SINH function - returns the hyperbolic sine
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for SINH)
 * @return Hyperbolic sine
 */
Value sinh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief COSH function - returns the hyperbolic cosine
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for COSH)
 * @return Hyperbolic cosine
 */
Value cosh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief TANH function - returns the hyperbolic tangent
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for TANH)
 * @return Hyperbolic tangent
 */
Value tanh_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief DEGREES function - converts radians to degrees
 * @param args Function arguments (expects 1 numeric argument in radians)
 * @param context Evaluation context (unused for DEGREES)
 * @return Angle in degrees
 */
Value degrees_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief RADIANS function - converts degrees to radians
 * @param args Function arguments (expects 1 numeric argument in degrees)
 * @param context Evaluation context (unused for RADIANS)
 * @return Angle in radians
 */
Value radians_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief EXP function - returns e raised to a power
 * @param args Function arguments (expects 1 numeric argument)
 * @param context Evaluation context (unused for EXP)
 * @return e raised to the power
 */
Value exp_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LN function - returns the natural logarithm
 * @param args Function arguments (expects 1 positive numeric argument)
 * @param context Evaluation context (unused for LN)
 * @return Natural logarithm
 */
Value ln_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOG function - returns the logarithm to a specified base
 * @param args Function arguments (expects 1 or 2 arguments: number, [base])
 * @param context Evaluation context (unused for LOG)
 * @return Logarithm to the specified base (default 10)
 */
Value log_function(const std::vector<Value>& args, const Context& context);

/**
 * @brief LOG10 function - returns the base-10 logarithm
 * @param args Function arguments (expects 1 positive numeric argument)
 * @param context Evaluation context (unused for LOG10)
 * @return Base-10 logarithm
 */
Value log10_function(const std::vector<Value>& args, const Context& context);

}  // namespace builtin

/**
 * @brief Utility functions for argument validation
 */
namespace utils {

/**
 * @brief Validate minimum number of arguments
 * @param args Arguments to validate
 * @param min_count Minimum required arguments
 * @param function_name Function name for error messages
 * @return Error value if validation fails, empty value if success
 */
Value validateMinArgs(const std::vector<Value>& args, size_t min_count,
                      const std::string& function_name);

/**
 * @brief Validate exact number of arguments
 * @param args Arguments to validate
 * @param count Required number of arguments
 * @param function_name Function name for error messages
 * @return Error value if validation fails, empty value if success
 */
Value validateArgCount(const std::vector<Value>& args, size_t count,
                       const std::string& function_name);

/**
 * @brief Check if any argument is an error
 * @param args Arguments to check
 * @return First error found, or empty value if no errors
 */
Value checkForErrors(const std::vector<Value>& args);

/**
 * @brief Convert value to number, handling errors
 * @param value Value to convert
 * @param function_name Function name for error messages
 * @return Converted number or error value
 */
Value toNumberSafe(const Value& value, const std::string& function_name);

}  // namespace utils

// Template function declarations for common patterns
namespace templates {

/**
 * @brief Template for single-argument numeric functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The numeric operation to perform
 * @return Result of the operation
 */
template<typename Func>
Value singleNumericFunction(const std::vector<Value>& args, const Context& context,
                           const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    
    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }
    
    auto num = utils::toNumberSafe(args[0], name);
    if (num.isError()) {
        return num;
    }
    
    try {
        return Value(operation(num.asNumber()));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::NUM_ERROR);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for multi-argument numeric functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The numeric operation to perform
 * @return Result of the operation
 */
template<typename Func>
Value multiNumericFunction(const std::vector<Value>& args, const Context& context,
                          const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter
    
    // Allow empty arguments for functions like SUM
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    std::vector<double> numbers;
    numbers.reserve(args.size());
    
    for (const auto& arg : args) {
        if (!arg.isEmpty() && arg.canConvertToNumber()) {
            numbers.push_back(arg.toNumber());
        }
    }
    
    if (numbers.empty()) {
        return Value(0.0);
    }
    
    try {
        return Value(operation(numbers));
    } catch (const std::runtime_error&) {
        return Value::error(ErrorType::DIV_ZERO);
    } catch (...) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
}

/**
 * @brief Template for single-argument text functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The text operation to perform
 * @return Result of the operation
 */
template<typename Func>
Value singleTextFunction(const std::vector<Value>& args, const Context& context,
                        const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    
    auto validation = utils::validateArgCount(args, 1, name);
    if (!validation.isEmpty()) {
        return validation;
    }
    
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    // Convert to text if needed
    std::string text;
    if (args[0].isText()) {
        text = args[0].asText();
    } else {
        text = args[0].toString();
    }
    
    return Value(operation(text));
}

/**
 * @brief Template for no-argument functions
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform
 * @return Result of the operation
 */
template<typename Func>
Value noArgFunction(const std::vector<Value>& args, const Context& context,
                   const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    
    auto validation = utils::validateArgCount(args, 0, name);
    if (!validation.isEmpty()) {
        return validation;
    }
    
    return operation();
}

/**
 * @brief Template for functions that take 1-2 arguments (like ROUND, FLOOR, CEILING, TRUNC)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform (takes value and optional second arg)
 * @return Result of the operation
 */
template<typename Func>
Value oneOrTwoArgFunction(const std::vector<Value>& args, const Context& context,
                         const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    
    // Validate argument count (1 or 2 arguments)
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    // Convert first argument to number
    auto number = utils::toNumberSafe(args[0], name);
    if (number.isError()) {
        return number;
    }
    
    double value = number.asNumber();
    
    // If second argument provided, convert it too
    if (args.size() == 2) {
        auto second = utils::toNumberSafe(args[1], name);
        if (second.isError()) {
            return second;
        }
        
        try {
            return Value(operation(value, second.asNumber()));
        } catch (const std::runtime_error&) {
            return Value::error(ErrorType::NUM_ERROR);
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    } else {
        // Only one argument
        try {
            return Value(operation(value, 0.0)); // Default second argument
        } catch (const std::runtime_error&) {
            return Value::error(ErrorType::NUM_ERROR);
        } catch (...) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
    }
}

/**
 * @brief Template for text functions that take 1-2 arguments (like LEFT, RIGHT)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The text operation to perform (takes text and optional num_chars)
 * @return Result of the operation
 */
template<typename Func>
Value oneOrTwoArgTextFunction(const std::vector<Value>& args, const Context& context,
                             const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter
    
    // Validate argument count (1 or 2 arguments)
    if (args.size() < 1 || args.size() > 2) {
        return Value::error(ErrorType::VALUE_ERROR);
    }
    
    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    // Convert first argument to text
    std::string text = args[0].toString();
    
    // If second argument provided, validate it's a number
    int num_chars = 1; // Default value
    if (args.size() == 2) {
        if (!args[1].isNumber()) {
            return Value::error(ErrorType::VALUE_ERROR);
        }
        num_chars = static_cast<int>(args[1].asNumber());
        
        // If num_chars is negative, return empty string
        if (num_chars < 0) {
            return Value("");
        }
    }
    
    return Value(operation(text, num_chars));
}

/**
 * @brief Template for functions that take any number of arguments (like CONCATENATE)
 * @param args Function arguments
 * @param context Evaluation context
 * @param name Function name for error messages
 * @param operation The operation to perform (takes all arguments)
 * @return Result of the operation
 */
template<typename Func>
Value multiArgFunction(const std::vector<Value>& args, const Context& context,
                      const std::string& name, Func operation) {
    (void)context;  // Unused parameter
    (void)name;     // Unused parameter
    
    // Check for errors first
    auto errorCheck = utils::checkForErrors(args);
    if (!errorCheck.isEmpty()) {
        return errorCheck;
    }
    
    return Value(operation(args));
}

}  // namespace templates

}  // namespace functions
}  // namespace xl_formula