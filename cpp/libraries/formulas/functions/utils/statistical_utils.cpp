#include "velox/formulas/statistical_utils.h"
#include "velox/formulas/functions.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace xl_formula {
namespace functions {
namespace utils {

/**
 * @brief Extract numeric values from a vector, filtering out non-numeric values
 * @param values Input vector of values
 * @return Vector containing only numeric values as doubles
 */
std::vector<double> extractNumericValues(const std::vector<Value>& values) {
    std::vector<double> numericValues;
    numericValues.reserve(values.size());
    
    for (const auto& value : values) {
        if (value.isNumber()) {
            numericValues.push_back(value.toNumber());
        }
    }
    
    return numericValues;
}

/**
 * @brief Validate that k is within valid range for array size
 * @param k The k value to validate (1-based)
 * @param arraySize Size of the array
 * @return true if k is valid, false otherwise
 */
bool validateKValue(size_t k, size_t arraySize) {
    return k > 0 && k <= arraySize;
}

/**
 * @brief Efficient k-th element selection using nth_element algorithm
 * @param array Vector of values to select from (passed by value for modification)
 * @param k Index of element to select (0-based)
 * @param ascending If true, selects k-th smallest; if false, k-th largest
 * @return The k-th element value
 * @throws std::invalid_argument if array is empty, contains no numeric values, or k is out of range
 */
Value quickSelect(std::vector<Value> array, size_t k, bool ascending) {
    if (array.empty()) {
        throw std::invalid_argument("Array cannot be empty");
    }
    
    // Extract numeric values only
    std::vector<double> numericArray = extractNumericValues(array);
    
    if (numericArray.empty()) {
        throw std::invalid_argument("Array contains no numeric values");
    }
    
    if (!validateKValue(k + 1, numericArray.size())) { // k is 0-based here, but validation expects 1-based
        throw std::invalid_argument("k value is out of range");
    }
    
    // For simplicity and correctness, we'll use partial_sort instead of implementing quickselect
    // This ensures stable behavior matching Excel
    if (ascending) {
        std::nth_element(numericArray.begin(), numericArray.begin() + k, numericArray.end());
    } else {
        std::nth_element(numericArray.begin(), numericArray.begin() + k, numericArray.end(), std::greater<double>());
    }
    
    return Value(numericArray[k]);
}

/**
 * @brief Calculate percentile with linear interpolation
 * @param array Vector of values (passed by value for modification)
 * @param percentile Percentile value (0.0 to 1.0)
 * @return Calculated percentile value
 * @throws std::invalid_argument if array is empty, contains no numeric values, or percentile is out of range
 */
Value calculatePercentile(std::vector<Value> array, double percentile) {
    if (array.empty()) {
        throw std::invalid_argument("Array cannot be empty");
    }
    
    if (percentile < 0.0 || percentile > 1.0) {
        throw std::invalid_argument("Percentile must be between 0 and 1");
    }
    
    // Extract and sort numeric values
    std::vector<double> numericArray = extractNumericValues(array);
    
    if (numericArray.empty()) {
        throw std::invalid_argument("Array contains no numeric values");
    }
    
    std::sort(numericArray.begin(), numericArray.end());
    
    // Calculate position using Excel's method
    double position = percentile * (numericArray.size() - 1);
    size_t lowerIndex = static_cast<size_t>(std::floor(position));
    size_t upperIndex = static_cast<size_t>(std::ceil(position));
    
    // Handle edge cases
    if (lowerIndex >= numericArray.size()) {
        return Value(numericArray.back());
    }
    
    if (lowerIndex == upperIndex) {
        return Value(numericArray[lowerIndex]);
    }
    
    // Linear interpolation
    double fraction = position - lowerIndex;
    double result = numericArray[lowerIndex] + fraction * (numericArray[upperIndex] - numericArray[lowerIndex]);
    
    return Value(result);
}

/**
 * @brief Calculate rank of a number in an array
 * @param number The number to rank
 * @param array Vector of values to rank against
 * @param ascending If true, ranks in ascending order; if false, descending
 * @return Rank of the number (1-based)
 * @throws std::invalid_argument if array is empty, contains no numeric values, number is not numeric, or number not found
 */
Value calculateRank(const Value& number, const std::vector<Value>& array, bool ascending) {
    if (array.empty()) {
        throw std::invalid_argument("Array cannot be empty");
    }
    
    if (!number.isNumber()) {
        throw std::invalid_argument("Number to rank must be numeric");
    }
    
    double targetNumber = number.toNumber();
    std::vector<double> numericArray = extractNumericValues(array);
    
    if (numericArray.empty()) {
        throw std::invalid_argument("Array contains no numeric values");
    }
    
    // Count how many values are better than the target
    size_t rank = 1;
    
    for (double value : numericArray) {
        if (ascending) {
            if (value < targetNumber) {
                rank++;
            }
        } else {
            if (value > targetNumber) {
                rank++;
            }
        }
    }
    
    // Check if the target number exists in the array
    bool found = std::find_if(numericArray.begin(), numericArray.end(),
                             [targetNumber](double val) { return std::abs(val - targetNumber) < 1e-10; }) != numericArray.end();
    
    if (!found) {
        throw std::invalid_argument("Number not found in array");
    }
    
    return Value(static_cast<double>(rank));
}

} // namespace utils
} // namespace functions
} // namespace xl_formula