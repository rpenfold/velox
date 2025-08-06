# Changelog

All notable changes to the XL-Formula library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **Phase 3: Logical Functions** - Complete implementation of all Phase 3 logical functions
  - `AND(...)` - Returns TRUE if all arguments are TRUE
  - `OR(...)` - Returns TRUE if any argument is TRUE  
  - `NOT(value)` - Reverses the logic of its argument
  - `XOR(...)` - Returns TRUE if an odd number of arguments are TRUE
  - `IFERROR(value, value_if_error)` - Returns a value if expression is an error
  - `IFNA(value, value_if_na)` - Returns a value if expression is #N/A
  - `ISNUMBER(value)` - Tests if a value is a number
  - `ISTEXT(value)` - Tests if a value is text
  - `ISBLANK(value)` - Tests if a value is blank
  - `ISERROR(value)` - Tests if a value is an error

### Changed
- Updated function registry to include all new Phase 3 logical functions
- Added comprehensive test coverage for all new logical functions
- Enhanced function documentation with detailed parameter descriptions

### Technical Details
- All logical functions follow consistent error handling patterns
- Boolean conversion logic handles numbers, text, and empty values correctly
- Error propagation works correctly across all logical functions
- Type checking functions properly validate input types
- Conditional functions handle edge cases and error conditions appropriately

## [0.1.0] - Initial Release

### Added
- Core formula parsing and evaluation engine
- Basic arithmetic operations (+, -, *, /, ^)
- Comparison operators (=, <>, <, <=, >, >=)
- Text concatenation (&)
- Variable support with Context class
- Phase 1: Math Functions (SUM, MAX, MIN, AVERAGE, etc.)
- Phase 2: Text Functions (CONCATENATE, TRIM, LEN, LEFT, RIGHT, etc.)
- Phase 3: Basic Logical Functions (TRUE, FALSE, IF)

### Technical Details
- High-performance C++17 implementation
- Comprehensive test suite with >95% coverage
- Excel-compatible syntax and function names
- Extensible function registration system
- Cross-platform build support