# Statistical Functions Implementation - Phase 1: Basic Statistical Functions

## Overview

This phase implements fundamental statistical functions that form the foundation for data analysis capabilities. These functions are essential for basic data analysis and are commonly used in financial and scientific calculations.

## Proposed Functions

### Basic Statistical Functions
1. `LARGE(array, k)` - Returns the k-th largest value in a dataset
2. `SMALL(array, k)` - Returns the k-th smallest value in a dataset
3. `RANK(number, ref, [order])` - Returns the rank of a number in a list of numbers
4. `PERCENTILE(array, k)` - Returns the k-th percentile of values in a range
5. `QUARTILE(array, quart)` - Returns the quartile of a data set

## Technical Design

### Core Statistical Utilities
```cpp
// statistical_utils.h
namespace xl_formula {
namespace functions {
namespace utils {

// Efficient k-th element selection (for LARGE/SMALL)
Value quickSelect(const std::vector<Value>& array, size_t k, bool ascending);

// Percentile calculation with interpolation
Value calculatePercentile(const std::vector<Value>& array, double percentile);

// Rank calculation with handling for duplicates
Value calculateRank(const Value& number, const std::vector<Value>& array, bool ascending);

} // namespace utils
} // namespace functions
} // namespace xl_formula
```

## Implementation Steps

### 1. Core Statistical Utilities (2 days)
- [ ] Create `statistical_utils.h` and `statistical_utils.cpp` in `functions/utils/`
  ```cpp
  // Key algorithms to implement
  - quickSelect algorithm for k-th element selection
  - Percentile calculation with linear interpolation
  - Rank calculation with duplicate handling
  ```
- [ ] Add comprehensive unit tests
  ```cpp
  TEST(StatisticalUtils, QuickSelect) {
    // Test cases for k-th element selection
  }
  TEST(StatisticalUtils, Percentile) {
    // Test cases for percentile calculation
  }
  TEST(StatisticalUtils, Rank) {
    // Test cases for rank calculation
  }
  ```

### 2. LARGE and SMALL Functions (1 day)
- [ ] Create `large.cpp` and `small.cpp` in `functions/math/`
  ```cpp
  Value large(const std::vector<Value>& args, const Context& context) {
    // 1. Validate arguments
    // 2. Extract and validate k
    // 3. Process array using quickSelect
    // 4. Return result
  }
  ```
- [ ] Add unit tests
  ```cpp
  TEST(StatisticalFunctions, Large) {
    // Test basic functionality
    // Test edge cases
    // Test error conditions
  }
  ```

### 3. RANK Function (1 day)
- [ ] Create `rank.cpp` in `functions/math/`
  ```cpp
  Value rank(const std::vector<Value>& args, const Context& context) {
    // 1. Validate arguments
    // 2. Extract number and reference array
    // 3. Calculate rank using utils
    // 4. Handle order parameter
  }
  ```
- [ ] Add unit tests
  ```cpp
  TEST(StatisticalFunctions, Rank) {
    // Test ascending/descending
    // Test duplicates
    // Test error cases
  }
  ```

### 4. PERCENTILE and QUARTILE Functions (1 day)
- [ ] Create `percentile.cpp` and `quartile.cpp` in `functions/math/`
  ```cpp
  Value percentile(const std::vector<Value>& args, const Context& context) {
    // 1. Validate arguments
    // 2. Extract percentile value
    // 3. Calculate using utils
    // 4. Handle interpolation
  }
  ```
- [ ] Add unit tests
  ```cpp
  TEST(StatisticalFunctions, Percentile) {
    // Test basic calculations
    // Test interpolation
    // Test edge cases
  }
  ```

### 5. Integration and Documentation (1 day)
- [ ] Update function registry in `fn_dispatcher.cpp`
- [ ] Add integration tests
- [ ] Update documentation
- [ ] Add performance benchmarks

## Dependencies

### Required Components
- Core math utilities
- Array handling functions
- Error handling system

### Required Knowledge
- Statistical algorithms
- Excel function behavior
- C++ optimization techniques

## Testing Strategy

### 1. Unit Tests
- Individual function behavior
- Edge cases
- Error conditions

### 2. Integration Tests
- Interaction with other functions
- Complex formulas

### 3. Performance Tests
- Large datasets
- Repeated calculations

## Validation

### Excel Compatibility
Compare results with Excel for:
- Basic calculations
- Edge cases
- Error handling
- Large datasets

### Performance Benchmarks
- Measure execution time
- Memory usage
- Compare with Excel performance

## Timeline

Total: 6 working days
1. Core utilities: 2 days
2. LARGE/SMALL: 1 day
3. RANK: 1 day
4. PERCENTILE/QUARTILE: 1 day
5. Integration/Docs: 1 day

## Future Considerations

### Optimization Opportunities
- SIMD operations for large arrays
- Parallel processing for large datasets
- Caching of sorted arrays

### Extension Points
- Additional statistical functions
- Custom aggregation functions
- Array operation optimizations
