#pragma once

#include "types.h"
#include <vector>

namespace xl_formula {
namespace functions {
namespace utils {

/**
 * @brief Statistical utilities for basic statistical functions
 * 
 * This header provides core algorithms for statistical calculations
 * including k-th element selection, percentile calculation, and ranking.
 */

/**
 * @brief Efficient k-th element selection using quickselect algorithm
 * @param array Vector of values to select from
 * @param k Index of element to select (0-based)
 * @param ascending If true, selects k-th smallest; if false, k-th largest
 * @return The k-th element value
 */
Value quickSelect(std::vector<Value> array, size_t k, bool ascending = true);

/**
 * @brief Calculate percentile with linear interpolation
 * @param array Vector of values
 * @param percentile Percentile value (0.0 to 1.0)
 * @return Calculated percentile value
 */
Value calculatePercentile(std::vector<Value> array, double percentile);

/**
 * @brief Calculate rank of a number in an array
 * @param number The number to rank
 * @param array Vector of values to rank against
 * @param ascending If true, ranks in ascending order; if false, descending
 * @return Rank of the number (1-based)
 */
Value calculateRank(const Value& number, const std::vector<Value>& array, bool ascending = true);

/**
 * @brief Extract numeric values from a vector, filtering out non-numeric values
 * @param values Input vector of values
 * @return Vector containing only numeric values
 */
std::vector<double> extractNumericValues(const std::vector<Value>& values);

/**
 * @brief Validate that k is within valid range for array size
 * @param k The k value to validate
 * @param arraySize Size of the array
 * @return true if k is valid, false otherwise
 */
bool validateKValue(size_t k, size_t arraySize);

} // namespace utils
} // namespace functions
} // namespace xl_formula