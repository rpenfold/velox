#pragma once

#include "types.h"

// Forward declaration
namespace xl_formula {
namespace functions {
bool simpleWildcardMatch(const std::string& text, const std::string& pattern);
}
}

namespace xl_formula {
namespace conditional {

/**
 * @brief Shared utilities for conditional functions and criteria evaluation
 * 
 * This header consolidates common patterns used across IF, SUMIF, COUNTIF, 
 * AVERAGEIF and related functions to reduce code duplication and improve
 * maintainability.
 */

/**
 * @brief Convert a Value to boolean using Excel rules
 * @param value The value to convert
 * @return Boolean result following Excel conversion rules
 */
bool toBooleanExcel(const Value& value);

/**
 * @brief Generic error-based conditional function template
 * @param value The value to check
 * @param value_if_condition The value to return if condition is met
 * @param error_predicate Function that returns true for target error type
 * @return value_if_condition if error condition met, otherwise value
 */
template<typename ErrorPredicate>
Value conditionalOnError(const Value& value, const Value& value_if_condition, ErrorPredicate predicate) {
    if (value.isError() && predicate(value.asError())) {
        return value_if_condition;
    }
    return value;
}

/**
 * @brief Unified criteria evaluation for *IF functions
 * @param value The value to test against criteria
 * @param criteria The criteria (can be number, text with operators, wildcards)
 * @return true if value matches criteria
 */
bool evaluateCriteria(const Value& value, const Value& criteria);

/**
 * @brief Template for single-criteria functions (SUMIF, COUNTIF, AVERAGEIF)
 * @param args Function arguments
 * @param context Evaluation context
 * @param function_name Function name for error messages
 * @param operation Callback that processes matching values
 * @return Result of the operation
 */
template<typename Operation>
Value singleCriteriaFunction(const std::vector<Value>& args, const Context& context,
                             const std::string& function_name, Operation operation);

/**
 * @brief Template for multi-criteria functions (SUMIFS, AVERAGEIFS)
 * @param args Function arguments
 * @param context Evaluation context  
 * @param function_name Function name for error messages
 * @param operation Callback that processes matching values
 * @return Result of the operation
 */
template<typename Operation>
Value multiCriteriaFunction(const std::vector<Value>& args, const Context& context,
                           const std::string& function_name, Operation operation);

/**
 * @brief Check if all criteria pairs match for multi-criteria functions
 * @param args Function arguments (starting from criteria pairs)
 * @param start_index Index where criteria pairs start
 * @return true if all criteria match, false otherwise
 */
bool evaluateAllCriteria(const std::vector<Value>& args, size_t start_index);

}  // namespace conditional
}  // namespace xl_formula