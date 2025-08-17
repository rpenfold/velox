# Contributing to Velox

Thank you for your interest in contributing to Velox! This document provides concise guidelines for developers who want to contribute.

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

5. **Platform Bindings**
   - Emscripten (for web builds)
   - React Native development environment (future mobile builds)

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
   git clone https://github.com/your-username/velox.git
   cd velox
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
| `--web` | Build web bindings | OFF |
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
│       ├── datetime/      # Date & time functions
│       ├── financial/     # Financial functions
│       ├── engineering/   # Engineering functions
│       ├── utils/         # Utility functions
│       └── fn_dispatcher.cpp # Perfect hash function dispatcher
├── cpp/include/xl-formula/    # Public headers
├── tests/                 # Comprehensive test suite
├── examples/              # Usage examples
└── scripts/               # Build and utility scripts
```

## Adding New Functions

This section provides a step-by-step guide for adding new functions to XL-Formula. We follow a systematic approach to ensure consistency, testability, and maintainability.

### Doxygen Doc Comment Guidelines (for docs generation)

We auto-generate the function catalog used by the docs app by parsing concise Doxygen-style blocks in C++ files. Please include a block above each function implementation with:

- `@brief` One-line description
- `@ingroup` Category key (e.g., `math`, `text`, `logical`, `datetime`, `financial`, `engineering`). Choose the closest fit.
- `@name` Excel-style function name (uppercase). For non-standard functions use the `NS_` prefix (e.g., `NS_UNIXTIME`). The docs infer a Non-Standard badge when name starts with `NS_`.
- `@param NAME description` for each parameter
  - Mark optional params by including `(optional)` in the description
  - Mark variadic by including `(variadic)` on the variadic parameter
  - Default values can be hinted with `(default: X)` in the description
- `@code ... @endcode` Example lines. You can add either:
  - `FORMULA -> RESULT` lines, or
  - `formula=..., result=..., description=...` semicolon-separated lines

Example:

```cpp
/**
 * @brief Returns the k-th percentile of a dataset
 * @ingroup statistics
 * @name PERCENTILE
 * @param array Data values (variadic)
 * @param k Percentile fraction between 0 and 1
 * @code
 * PERCENTILE({1,2,3,4,5}, 0.5) -> 3
 * @endcode
 */
```

Notes:
- If `@name` is absent we infer the name from the function identifier (uppercased, trimming a trailing `_function`).
- If `@ingroup` is absent we fall back to the directory name under `cpp/functions/`.
- The docs generator removes `(optional)`, `(variadic)`, `(default: ...)` from the displayed parameter descriptions but uses them to build syntax and badges.

### Overview

Each function requires implementation in multiple places:
1. **C++ Implementation** - Core function logic
2. **Function Registration** - Add to dispatcher and name lists
3. **Tests** - Comprehensive test coverage
4. **Documentation** - Add to function catalog and examples
5. **Web Bindings** - Update JavaScript wrapper (if needed)

### Step 1: Choose Function Category

Functions are organized into categories based on their purpose:

- **`cpp/functions/math/`** - Mathematical and statistical functions
- **`cpp/functions/text/`** - Text manipulation functions
- **`cpp/functions/logical/`** - Logical and conditional functions
- **`cpp/functions/datetime/`** - Date and time functions
- **`cpp/functions/financial/`** - Financial calculations
- **`cpp/functions/engineering/`** - Engineering and conversion functions

### Step 2: Create Implementation File

Create a new `.cpp` file in the appropriate category directory. **Use function templates when possible** to reduce boilerplate code:

#### Option A: Using Function Templates (Recommended)

**Available Templates:**
- `templates::oneOrTwoArgTextFunction` - For LEFT, RIGHT, UPPER, LOWER (1-2 text args)
- `templates::twoArgTextNumberFunction` - For RPT, functions with text + number args
- `templates::multiArgFunction` - For CONCATENATE, SUM, functions with variable args
- `templates::singleNumericFunction` - For ABS, SQRT, single number functions
- `templates::twoNumericFunction` - For MOD, POWER, two number functions

**Example using template:**
```cpp
// cpp/functions/text/example_function.cpp
#include "xl-formula/functions.h"

namespace xl_formula {
namespace functions {
namespace builtin {

Value example_function(const std::vector<Value>& args, const Context& context) {
    return templates::twoArgTextNumberFunction(
        args, context, "EXAMPLE_FUNCTION", [](const std::string& text, double number) {
            // Your core logic here - validation is handled by the template
            return text + std::to_string(number);
        });
}

}}} // namespace xl_formula::functions::builtin
```

#### Option B: Manual Implementation (When templates don't fit)

```cpp
// cpp/functions/math/example_function.cpp
#include "xl-formula/functions.h"
#include <cmath>

namespace xl_formula {
namespace functions {
namespace builtin {

Value example_function(const std::vector<Value>& args, const Context& context) {
    // Validate arguments
    if (args.size() != 1) {
        throw std::runtime_error("EXAMPLE_FUNCTION requires exactly 1 argument");
    }
    
    // Get the argument
    const Value& arg = args[0];
    
    // Validate argument type
    if (!arg.isNumber()) {
        throw std::runtime_error("EXAMPLE_FUNCTION argument must be a number");
    }
    
    // Perform calculation
    double input = arg.asNumber();
    double result = std::sqrt(input); // Example calculation
    
    // Return result
    return Value(result);
}

}}} // namespace xl_formula::functions::builtin
```

### Step 3: Add Function Declaration

Add the function declaration to the appropriate header file:

```cpp
// cpp/include/xl-formula/functions.h
// Add in the appropriate namespace section:

// Math functions
Value example_function(const std::vector<Value>& args, const Context& context);
```

### Step 4: Register Function in Dispatcher

Update the function dispatcher to include your new function:

```cpp
// cpp/functions/fn_dispatcher.cpp

// Add to the switch statement in evaluate_function():
case hash_function_name("EXAMPLE_FUNCTION"): 
    return builtin::example_function(args, context);

// Add to get_builtin_function_names() in the appropriate category:
// Math functions section:
"EXAMPLE_FUNCTION",
```

### Step 5: Create Comprehensive Tests

Create a test file in the corresponding test directory:

```cpp
// tests/functions/math/test_example_function.cpp
#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/context.h"

class ExampleFunctionTest : public ::testing::Test {
protected:
    Context context;
    
    void SetUp() override {
        // Setup any test fixtures
    }
};

TEST_F(ExampleFunctionTest, ValidInput_ReturnsCorrectResult) {
    // Arrange
    std::vector<Value> args = {Value(4.0)};
    
    // Act
    Value result = builtin::example_function(args, context);
    
    // Assert
    EXPECT_TRUE(result.isNumber());
    EXPECT_DOUBLE_EQ(2.0, result.asNumber());
}

TEST_F(ExampleFunctionTest, InvalidArgumentCount_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value(1.0), Value(2.0)};
    
    // Act & Assert
    EXPECT_THROW(builtin::example_function(args, context), std::runtime_error);
}

TEST_F(ExampleFunctionTest, NonNumericArgument_ThrowsException) {
    // Arrange
    std::vector<Value> args = {Value("not a number")};
    
    // Act & Assert
    EXPECT_THROW(builtin::example_function(args, context), std::runtime_error);
}

TEST_F(ExampleFunctionTest, EdgeCases_HandledCorrectly) {
    // Test edge cases like zero, negative numbers, etc.
    std::vector<Value> args = {Value(0.0)};
    Value result = builtin::example_function(args, context);
    EXPECT_DOUBLE_EQ(0.0, result.asNumber());
}
```

### Step 6: Add to CMake Build System

Update the CMakeLists.txt file in the functions directory to include your new file:

```cmake
# cpp/functions/math/CMakeLists.txt
set(MATH_FUNCTIONS
    abs.cpp
    acos.cpp
    # ... existing functions ...
    example_function.cpp  # Add your new function here
)
```

### Step 7: Update Documentation

Add your function to the function catalog in the JavaScript file:

```javascript
// apps/docs/src/data/functions.js
// Add to the appropriate category:

text: {
    name: 'Text',
    description: 'Text manipulation and formatting functions',
    functions: {
        // ... existing functions ...
        EXAMPLE_FUNCTION: {
            name: 'EXAMPLE_FUNCTION',
            description: 'Example function that demonstrates the pattern',
            syntax: 'EXAMPLE_FUNCTION(text, number)',
            parameters: [
                { name: 'text', description: 'The text to process', required: true },
                { name: 'number', description: 'The number parameter', required: true }
            ],
            examples: [
                { 
                    formula: 'EXAMPLE_FUNCTION("abc", 3)', 
                    result: '"abc3"', 
                    description: 'Combines text with number' 
                },
                { 
                    formula: 'EXAMPLE_FUNCTION("x", 5)', 
                    result: '"x5"', 
                    description: 'Another example' 
                }
            ]
        }
    }
}
```

**Important Notes:**
- Use single quotes for strings in the JavaScript file
- Include the `parameters` array with `name`, `description`, and `required` fields
- Provide at least 2 examples with different scenarios
- Make sure the function appears in the correct category section
- The file structure uses JavaScript objects, not JSON

### Step 8: Test Integration

Build and test your changes:

```bash
# Build with tests
./build.sh --debug --tests

# Run specific test
cd build
./xl-formula-tests --gtest_filter="*ExampleFunction*"

# Test in examples
./examples/basic_example

# Test web bindings (if applicable)
cd ../../packages/web
npm test

# Test documentation
cd ../../apps/docs
npm run build
npm run dev  # Check if function appears in docs
```

### Step 9: Update Web Bindings (if needed)

If your function returns a new data type or has special behavior, you may need to update the web bindings:

```cpp
// cpp/bindings/web/web_bindings.cpp
// Usually no changes needed for standard functions
```

**Note:** Most functions work automatically through the existing `evaluate()` method. The web bindings are primarily for the core API (`FormulaEngine`, `Value`, `EvaluationResult`), not individual functions.

### Common Patterns

#### Mathematical Functions
```cpp
Value math_function(const std::vector<Value>& args, const Context& context) {
    // Validate argument count
    validate_argument_count(args, 1, "FUNCTION_NAME");
    
    // Get and validate argument
    double input = get_numeric_argument(args[0], "FUNCTION_NAME");
    
    // Perform calculation
    double result = std::some_math_function(input);
    
    return Value(result);
}
```

#### Text Functions
```cpp
Value text_function(const std::vector<Value>& args, const Context& context) {
    // Validate argument count
    validate_argument_count(args, 1, "FUNCTION_NAME");
    
    // Get and validate argument
    std::string input = get_text_argument(args[0], "FUNCTION_NAME");
    
    // Perform text manipulation
    std::string result = transform_text(input);
    
    return Value(result);
}
```

#### Logical Functions
```cpp
Value logical_function(const std::vector<Value>& args, const Context& context) {
    // Validate argument count
    validate_argument_count(args, 2, "FUNCTION_NAME");
    
    // Get and validate arguments
    bool arg1 = get_boolean_argument(args[0], "FUNCTION_NAME");
    bool arg2 = get_boolean_argument(args[1], "FUNCTION_NAME");
    
    // Perform logical operation
    bool result = arg1 && arg2; // Example: AND function
    
    return Value(result);
}
```

### Validation Utilities

Use the validation utilities for consistent error handling:

```cpp
#include "xl-formula/functions.h"

// Use utils namespace for validation
auto error = utils::validateArgCount(args, 2, "FUNCTION_NAME");
if (!error.isEmpty()) {
    return error;
}

// Check for errors in arguments
auto errorCheck = utils::checkForErrors(args);
if (!errorCheck.isEmpty()) {
    return errorCheck;
}

// Convert arguments safely
std::string text = args[0].toString();
double number = args[1].asNumber();
```

**Note:** The validation utilities are in the `utils::` namespace, not global functions.

### Testing Guidelines

1. **Test Coverage**: Aim for 100% coverage of your function
2. **Edge Cases**: Test boundary conditions, invalid inputs, and error cases
3. **Performance**: Ensure your function performs well with typical inputs
4. **Integration**: Test that your function works correctly with other functions

### Example: Complete Function Implementation

Here's a complete example of implementing the `RPT` function using templates:

```cpp
// cpp/functions/text/rpt.cpp
#include "xl-formula/functions.h"
#include <string>

namespace xl_formula {
namespace functions {
namespace builtin {

Value rpt(const std::vector<Value>& args, const Context& context) {
    return templates::twoArgTextNumberFunction(
        args, context, "RPT", [](const std::string& text, double repeat_count) {
            // Validate that repeat count is non-negative
            if (repeat_count < 0) {
                return std::string(); // Return empty string for negative counts
            }

            // Convert to integer (truncate decimal part)
            int count = static_cast<int>(repeat_count);

            // Build the repeated string
            std::string result;
            result.reserve(text.length() * count); // Pre-allocate for efficiency

            for (int i = 0; i < count; ++i) {
                result += text;
            }

            return result;
        });
}

}}} // namespace xl_formula::functions::builtin
```

```cpp
// tests/functions/text/test_rpt.cpp
#include <gtest/gtest.h>
#include "xl-formula/functions.h"
#include "xl-formula/xl-formula.h"

class RptFunctionTest : public ::testing::Test {
protected:
    Context context;
};

TEST_F(RptFunctionTest, ValidInput_ReturnsCorrectResult) {
    std::vector<Value> args = {Value("abc"), Value(3.0)};
    Value result = builtin::rpt(args, context);
    EXPECT_TRUE(result.isText());
    EXPECT_EQ("abcabcabc", result.asText());
}

TEST_F(RptFunctionTest, SingleRepeat_ReturnsOriginal) {
    std::vector<Value> args = {Value("hello"), Value(1.0)};
    Value result = builtin::rpt(args, context);
    EXPECT_EQ("hello", result.asText());
}

TEST_F(RptFunctionTest, ZeroRepeat_ReturnsEmpty) {
    std::vector<Value> args = {Value("test"), Value(0.0)};
    Value result = builtin::rpt(args, context);
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, NegativeRepeat_ReturnsEmpty) {
    std::vector<Value> args = {Value("test"), Value(-1.0)};
    Value result = builtin::rpt(args, context);
    EXPECT_EQ("", result.asText());
}

TEST_F(RptFunctionTest, DecimalRepeat_TruncatesToInteger) {
    std::vector<Value> args = {Value("x"), Value(2.7)};
    Value result = builtin::rpt(args, context);
    EXPECT_EQ("xx", result.asText());
}

TEST_F(RptFunctionTest, EmptyString_HandledCorrectly) {
    std::vector<Value> args = {Value(""), Value(5.0)};
    Value result = builtin::rpt(args, context);
    EXPECT_EQ("", result.asText());
}
```

### Final Checklist

Before submitting your function implementation:

- [ ] Function compiles without errors
- [ ] All tests pass
- [ ] Function is registered in dispatcher
- [ ] Function is added to name lists
- [ ] Documentation is added via doxygen-style comment on C++ function
- [ ] Function appears in the docs interface
- [ ] Code is properly formatted
- [ ] Edge cases are tested
- [ ] Error handling is comprehensive
- [ ] Performance is acceptable
- [ ] Integration with other functions works correctly
- [ ] Web bindings work (if applicable)
- [ ] Template usage is appropriate (if using templates)

## Common Issues and Troubleshooting

### Function Not Appearing in Documentation

**Problem:** Function is implemented but doesn't show up in the docs interface.

**Solutions:**
1. **Check for duplicate sections** - Ensure there's only one section per category in `functions.js`
2. **Verify syntax** - Use single quotes, not double quotes in the JavaScript file
3. **Check category placement** - Make sure the function is in the correct category object
4. **Rebuild docs** - Run `npm run build` in `apps/docs` directory
5. **Check console errors** - Look for JavaScript errors in the browser console

### Template Compilation Errors

**Problem:** Function templates don't compile or work as expected.

**Solutions:**
1. **Check template namespace** - Use `templates::` prefix, not just the function name
2. **Verify template parameters** - Ensure lambda function signature matches template expectations
3. **Check includes** - Make sure `xl-formula/functions.h` is included
4. **Review template documentation** - See available templates list above

### Web Bindings Issues

**Problem:** Function works in C++ but not in JavaScript.

**Solutions:**
1. **Check function registration** - Ensure function is in `get_builtin_function_names()`
2. **Verify dispatcher** - Check that function is in the switch statement
3. **Test with `evaluate()`** - Use `engine.evaluate("FUNCTION_NAME(...)")` not direct calls
4. **Check error handling** - Ensure function returns proper `Value` objects

### Testing Issues

**Problem:** Tests fail or don't compile.

**Solutions:**
1. **Check include paths** - Use `#include "xl-formula/xl-formula.h"` for tests
2. **Verify test structure** - Follow the Arrange-Act-Assert pattern
3. **Check namespace** - Use `builtin::` namespace for function calls in tests
4. **Review test examples** - See the RPT test example above

## Parity and Roadmap

Moved to `docs/ROADMAP.md`.


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

- **Issues**: Report bugs or request features via GitHub Issues
- **Questions**: Use GitHub Discussions for questions and community support

## License

By contributing to Velox, you agree that your contributions will be licensed under the MIT License.