# Contributing to XL-Formula

Thank you for your interest in contributing to XL-Formula! This document provides comprehensive guidelines for developers who want to contribute to this project.

## Table of Contents

- [Development Environment Setup](#development-environment-setup)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
- [Code Style and Formatting](#code-style-and-formatting)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Function Implementation Roadmap](#function-implementation-roadmap)
- [Submitting Changes](#submitting-changes)
- [Release Process](#release-process)

## Development Environment Setup

### Prerequisites

Before you begin, ensure you have the following tools installed:

#### Required Dependencies

1. **C++ Compiler** (C++17 or later)
   ```bash
   # macOS (Xcode Command Line Tools)
   xcode-select --install
   
   # Ubuntu/Debian
   sudo apt-get install build-essential
   
   # Fedora/RHEL
   sudo dnf install gcc-c++ make
   ```

2. **CMake** (3.15 or later)
   ```bash
   # macOS
   brew install cmake
   
   # Ubuntu/Debian
   sudo apt-get install cmake
   
   # Fedora/RHEL
   sudo dnf install cmake
   ```

3. **Git**
   ```bash
   # macOS
   brew install git
   
   # Ubuntu/Debian
   sudo apt-get install git
   
   # Fedora/RHEL
   sudo dnf install git
   ```

#### Code Formatting

4. **clang-format** (for consistent code style)
   ```bash
   # macOS
   brew install clang-format
   
   # Ubuntu/Debian
   sudo apt-get install clang-format
   
   # Fedora/RHEL
   sudo dnf install clang-tools-extra
   ```

#### Optional Dependencies

5. **Future Dependencies** (not yet implemented)
   - Emscripten (for planned web builds)
   - React Native development environment (for planned mobile builds)

6. **Coverage Tools** (for coverage reports)
   ```bash
   # macOS
   brew install gcovr
   
   # Ubuntu/Debian
   sudo apt-get install gcovr
   
   # Fedora/RHEL
   sudo dnf install gcovr
   ```

### Getting Started

1. **Fork and Clone the Repository**
   ```bash
   git clone https://github.com/your-username/xl-formula.git
   cd xl-formula
   ```

2. **Initial Build and Test**
   ```bash
   # Build in debug mode with tests
   ./build.sh --debug --tests
   
   # Verify everything works
   ./build.sh --format-check
   ```

## Building the Project

### Quick Build Commands

```bash
# Basic release build
./build.sh

# Debug build with tests
./build.sh --debug --tests

# Clean build with coverage
./build.sh --clean --coverage --tests

# Format all code
./build.sh --format

# Check formatting (CI-friendly)
./build.sh --format-check
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `-h, --help` | Show help message | - |
| `-d, --debug` | Build in Debug mode | Release |
| `-c, --clean` | Clean build directory before building | false |
| `-t, --tests` | Build and run tests | false |
| `-e, --examples` | Build examples | ON |
| `--no-tests` | Don't build tests | - |
| `--no-examples` | Don't build examples | - |
| `--web` | Build web bindings (not implemented) | OFF |
| `--react-native` | Build React Native bindings (not implemented) | OFF |
| `--install` | Install after building | false |
| `--coverage` | Build with coverage information | false |
| `--format` | Format all C++ source files | false |
| `--format-check` | Check if files are properly formatted | false |

### Manual Build Process

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
make -j$(nproc)

# Run tests
ctest

# Run examples
./examples/basic_example
./examples/advanced_example
```

## Code Style and Formatting

### Automatic Formatting

This project uses `clang-format` for consistent code style:

```bash
# Format all C++ files
./build.sh --format

# Check if files need formatting (useful for CI)
./build.sh --format-check

# Direct script access with more options
./scripts/format.sh --help
./scripts/format.sh --dry-run    # Preview changes
./scripts/format.sh --verbose    # Detailed output
```

### IDE Integration

#### VS Code
1. Install the `C/C++` extension
2. Enable "Format on Save" in settings
3. The project's `.clang-format` will be automatically detected

#### CLion
1. Go to `Settings > Editor > Code Style > C/C++`
2. Set scheme to "Project"
3. Enable "Reformat code" in commit options

#### Vim/Neovim
1. Install `vim-clang-format` plugin
2. Use `:ClangFormat` command or set up auto-formatting

### Code Style Guidelines

- **Indentation**: 4 spaces (no tabs)
- **Line Length**: 100 characters maximum
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `FormulaEngine`)
  - Functions: `snake_case` (e.g., `evaluate_expression`)
  - Variables: `snake_case` (e.g., `result_value`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_RECURSION_DEPTH`)
- **File Organization**: One class per file, matching filenames
- **Include Order**: System headers, third-party headers, project headers
- **Documentation**: Doxygen-style comments for public APIs

## Testing

### Running Tests

```bash
# Run all tests
./build.sh --debug --tests

# Run specific test suite
cd build
./xl-formula-tests --gtest_filter="*SumFunction*"

# Run with verbose output
./xl-formula-tests --gtest_filter="*SumFunction*" --gtest_output=verbose

# Generate coverage report
./build.sh --coverage --tests
open coverage.html  # View coverage report
```

### Test Organization

Tests are organized by functionality:

```
tests/
├── functions/           # Function-specific tests
│   ├── math/           # Mathematical functions
│   ├── text/           # Text manipulation functions
│   ├── logical/        # Logical functions
│   └── utils/          # Utility function tests
├── test_types.cpp      # Core type system tests
├── test_parser.cpp     # Parser and lexer tests
├── test_evaluator.cpp  # Evaluator tests
└── test_integration.cpp # End-to-end integration tests
```

### Writing Tests

#### Test Naming Convention

Use descriptive test names that clearly indicate the scenario and expected outcome:

```cpp
TEST_F(SumFunctionTest, EmptyArguments_ReturnsZero) {
    // Test implementation
}

TEST_F(SumFunctionTest, MixedNumericTypes_ConvertsBooleans) {
    // Test implementation
}
```

#### Test Structure

Follow the Arrange-Act-Assert pattern:

```cpp
TEST_F(FunctionTest, Scenario_ExpectedBehavior) {
    // Arrange
    Context context;
    std::vector<Value> args = {Value(1.0), Value(2.0)};
    
    // Act
    Value result = function_under_test(args, context);
    
    // Assert
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(3.0, result.asNumber());
}
```

### Coverage Requirements

- **Minimum Coverage**: 95% for all new code
- **Test Types Required**:
  - Unit tests for all functions
  - Integration tests for complex workflows
  - Error handling tests
  - Edge case tests
  - Performance regression tests

## Project Structure

```
xl-formula/
├── cpp/                    # Core C++ implementation
│   ├── core/              # Value types, Context, API
│   │   ├── api.cpp        # Main API implementation
│   │   ├── context.cpp    # Variable context management
│   │   └── types.cpp      # Value type system
│   ├── parser/            # Lexer, Parser, AST
│   │   ├── ast.cpp        # Abstract syntax tree
│   │   ├── lexer.cpp      # Tokenization
│   │   └── parser.cpp     # Expression parsing
│   ├── engine/            # Evaluator, FormulaEngine
│   │   ├── evaluator.cpp      # AST evaluation
│   │   └── formula_engine.cpp # High-level engine
│   └── functions/         # Built-in functions
│       ├── math/          # Mathematical functions
│       ├── text/          # Text manipulation functions
│       ├── logical/       # Logical functions
│       ├── utils/         # Utility functions
│       └── function_registry.cpp # Function registration
├── include/xl-formula/    # Public headers
├── tests/                 # Comprehensive test suite
├── examples/              # Usage examples
└── scripts/               # Build and utility scripts
```

### Adding New Functions

1. **Create Implementation File**:
   ```cpp
   // cpp/functions/math/new_function.cpp
   #include "xl-formula/functions.h"
   #include "validation.h"
   
   namespace xl_formula {
   namespace functions {
   namespace builtin {
   
   Value new_function(const std::vector<Value>& args, const Context& context) {
       // Implementation
   }
   
   }}} // namespace xl_formula::functions::builtin
   ```

2. **Add Declaration to Header**:
   ```cpp
   // include/xl-formula/functions.h
   Value new_function(const std::vector<Value>& args, const Context& context);
   ```

3. **Register Function**:
   ```cpp
   // cpp/functions/function_registry.cpp
   registry->registerFunction("NEW_FUNCTION", functions::builtin::new_function);
   ```

4. **Create Tests**:
   ```cpp
   // tests/functions/math/test_new_function.cpp
   class NewFunctionTest : public ::testing::Test {
       // Test implementation
   };
   ```

## Function Implementation Roadmap

**Target**: Match and exceed Formula.js (399/515 functions implemented)
**Current**: 16 functions implemented

### Phase 1: Core Math & Statistical Functions (Priority: High)
- [x] **MIN** - Returns the minimum value in a set of values ✅
- [x] **AVERAGE** - Returns the average of its arguments ✅
- [x] **COUNT** - Counts the number of cells that contain numbers ✅
- [x] **COUNTA** - Counts the number of cells that are not empty ✅
- [x] **SQRT** - Returns the square root of a number ✅
- [x] **POWER** - Returns the result of a number raised to a power ✅
- [x] **MOD** - Returns the remainder from division ✅
- [ ] **COUNTIF** - Counts the number of cells that meet a criterion
- [ ] **PI** - Returns the value of pi
- [ ] **RAND** - Returns a random number between 0 and 1
- [ ] **RANDBETWEEN** - Returns a random number between specified numbers
- [ ] **SIGN** - Returns the sign of a number
- [ ] **INT** - Rounds a number down to the nearest integer
- [ ] **TRUNC** - Truncates a number to an integer
- [ ] **CEILING** - Rounds a number up to the nearest multiple
- [ ] **FLOOR** - Rounds a number down to the nearest multiple
- [ ] **MEDIAN** - Returns the median of a set of numbers
- [ ] **MODE** - Returns the most frequently occurring value
- [ ] **STDEV** - Returns the standard deviation
- [ ] **VAR** - Returns the variance

### Phase 2: Text Functions (Priority: High)
- [x] **CONCATENATE** - Joins several text strings ✅
- [x] **TRIM** - Removes extra spaces ✅
- [x] **LEN** - Returns the length of a text string ✅
- [ ] **LEFT** - Returns characters from the left side
- [ ] **RIGHT** - Returns characters from the right side
- [ ] **MID** - Returns characters from the middle
- [ ] **UPPER** - Converts text to uppercase
- [ ] **LOWER** - Converts text to lowercase
- [ ] **PROPER** - Capitalizes the first letter of each word
- [ ] **FIND** - Finds one text string within another
- [ ] **SEARCH** - Finds one text string within another (case-insensitive)
- [ ] **REPLACE** - Replaces part of a text string
- [ ] **SUBSTITUTE** - Substitutes new text for old text
- [ ] **TEXT** - Formats a number as text
- [ ] **VALUE** - Converts a text string to a number

### Phase 3: Logical Functions (Priority: Medium)
- [x] **TRUE** - Returns the logical value TRUE ✅
- [x] **FALSE** - Returns the logical value FALSE ✅
- [x] **IF** - Performs a conditional test ✅
- [ ] **AND** - Returns TRUE if all arguments are TRUE
- [ ] **OR** - Returns TRUE if any argument is TRUE
- [ ] **NOT** - Reverses the logic of its argument
- [ ] **XOR** - Returns TRUE if an odd number of arguments are TRUE
- [ ] **IFERROR** - Returns a value if expression is an error
- [ ] **IFNA** - Returns a value if expression is #N/A
- [ ] **ISNUMBER** - Tests if a value is a number
- [ ] **ISTEXT** - Tests if a value is text
- [ ] **ISBLANK** - Tests if a value is blank
- [ ] **ISERROR** - Tests if a value is an error

### Phase 4: Lookup & Reference Functions (Priority: Medium)
- [ ] **VLOOKUP** - Looks up a value in a table
- [ ] **HLOOKUP** - Looks up a value in a table (horizontal)
- [ ] **INDEX** - Returns a value from a table
- [ ] **MATCH** - Finds the position of a value
- [ ] **CHOOSE** - Chooses a value from a list
- [ ] **LOOKUP** - Looks up a value
- [ ] **INDIRECT** - Returns a reference specified by text
- [ ] **OFFSET** - Returns a reference offset from a given reference
- [ ] **ROW** - Returns the row number
- [ ] **COLUMN** - Returns the column number

### Phase 5: Date & Time Functions (Priority: Low)
- [ ] **NOW** - Returns the current date and time
- [ ] **TODAY** - Returns the current date
- [ ] **DATE** - Creates a date from year, month, day
- [ ] **TIME** - Creates a time from hour, minute, second
- [ ] **YEAR** - Returns the year from a date
- [ ] **MONTH** - Returns the month from a date
- [ ] **DAY** - Returns the day from a date
- [ ] **HOUR** - Returns the hour from a time
- [ ] **MINUTE** - Returns the minute from a time
- [ ] **SECOND** - Returns the second from a time
- [ ] **WEEKDAY** - Returns the day of the week
- [ ] **DATEDIF** - Calculates the difference between dates

### Phase 6: Advanced Math & Trigonometry (Priority: Low)
- [ ] **SIN** - Returns the sine of an angle
- [ ] **COS** - Returns the cosine of an angle
- [ ] **TAN** - Returns the tangent of an angle
- [ ] **ASIN** - Returns the arcsine
- [ ] **ACOS** - Returns the arccosine
- [ ] **ATAN** - Returns the arctangent
- [ ] **ATAN2** - Returns the arctangent from x and y coordinates
- [ ] **SINH** - Returns the hyperbolic sine
- [ ] **COSH** - Returns the hyperbolic cosine
- [ ] **TANH** - Returns the hyperbolic tangent
- [ ] **DEGREES** - Converts radians to degrees
- [ ] **RADIANS** - Converts degrees to radians
- [ ] **EXP** - Returns e raised to a power
- [ ] **LN** - Returns the natural logarithm
- [ ] **LOG** - Returns the logarithm to a specified base
- [ ] **LOG10** - Returns the base-10 logarithm

### Phase 7: Financial Functions (Priority: Future)
- [ ] **PV** - Present value
- [ ] **FV** - Future value
- [ ] **PMT** - Payment amount
- [ ] **RATE** - Interest rate
- [ ] **NPER** - Number of periods
- [ ] **NPV** - Net present value
- [ ] **IRR** - Internal rate of return
- [ ] **MIRR** - Modified internal rate of return

### Phase 8: Engineering & Specialized Functions (Priority: Future)
- [ ] **CONVERT** - Converts units
- [ ] **HEX2DEC** - Converts hexadecimal to decimal
- [ ] **DEC2HEX** - Converts decimal to hexadecimal
- [ ] **BIN2DEC** - Converts binary to decimal
- [ ] **DEC2BIN** - Converts decimal to binary
- [ ] **BITAND** - Bitwise AND
- [ ] **BITOR** - Bitwise OR
- [ ] **BITXOR** - Bitwise XOR

## Submitting Changes

### Development Workflow

1. **Create a Feature Branch**
   ```bash
   git checkout -b feature/new-awesome-function
   ```

2. **Make Your Changes**
   - Implement the feature
   - Add comprehensive tests
   - Update documentation
   - Format code: `./build.sh --format`

3. **Test Your Changes**
   ```bash
   # Run all tests
   ./build.sh --debug --tests
   
   # Check coverage
   ./build.sh --coverage --tests
   
   # Verify formatting
   ./build.sh --format-check
   ```

4. **Commit Guidelines**
   ```bash
   # Use conventional commit format
   git commit -m "feat: add COUNTIF function with condition parsing"
   git commit -m "fix: handle division by zero in MOD function"
   git commit -m "docs: update CONTRIBUTING.md with test guidelines"
   ```

5. **Submit Pull Request**
   - Ensure all tests pass
   - Include test coverage report
   - Update relevant documentation
   - Add examples if introducing new features

### Pull Request Checklist

- [ ] Tests pass locally (`./build.sh --debug --tests`)
- [ ] Code is properly formatted (`./build.sh --format-check`)
- [ ] Coverage is ≥95% (`./build.sh --coverage --tests`)
- [ ] Documentation is updated
- [ ] Commit messages follow conventional format
- [ ] No breaking changes (or clearly documented)
- [ ] Examples are provided for new features

### Code Review Process

1. **Automated Checks**: CI will verify build, tests, formatting, and coverage
2. **Peer Review**: At least one maintainer will review the code
3. **Integration Testing**: Changes will be tested against the full test suite
4. **Documentation Review**: Ensure all changes are properly documented

## Release Process

### Version Numbering

We follow [Semantic Versioning](https://semver.org/):
- **MAJOR**: Breaking changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

### Release Checklist

- [ ] All tests pass
- [ ] Coverage ≥95%
- [ ] Documentation updated
- [ ] Version bumped in CMakeLists.txt
- [ ] Changelog updated
- [ ] Release notes prepared
- [ ] Cross-platform build tested

## Getting Help

- **Issues**: Report bugs or request features via [GitHub Issues](https://github.com/your-org/xl-formula/issues)
- **Questions**: Use [GitHub Discussions](https://github.com/your-org/xl-formula/discussions) for questions and community support

## License

By contributing to XL-Formula, you agree that your contributions will be licensed under the MIT License.