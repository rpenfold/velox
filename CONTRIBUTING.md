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
   // cpp/include/xl-formula/functions.h
   Value new_function(const std::vector<Value>& args, const Context& context);
   ```

3. **Register Function in Dispatcher**:
   ```cpp
   // cpp/functions/fn_dispatcher.cpp - Add to switch statement
   case hash_function_name("NEW_FUNCTION"): return builtin::new_function(args, context);
   
   // cpp/functions/fn_dispatcher.cpp - Add to get_builtin_function_names()
   "NEW_FUNCTION",  // Add to appropriate category list
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
**Current**: 85 functions implemented (17% of Excel's ~500 core functions)

## Completed Functions ✅

**85 functions implemented across 8 categories:**
- **Math & Statistical**: 34 functions (SUM, AVERAGE, COUNT, SQRT, GCD, FACT, SUMIF, etc.)
- **Text**: 14 functions (CONCATENATE, TRIM, LEN, UPPER, etc.)
- **Logical**: 14 functions (IF, AND, OR, NOT, ISNUMBER, etc.)
- **Date & Time**: 12 functions (NOW, TODAY, DATE, YEAR, etc.)
- **Trigonometric**: 16 functions (SIN, COS, TAN, ATAN2, etc.)
- **Financial**: 8 functions (PV, FV, PMT, NPV, IRR, etc.)
- **Engineering**: 8 functions (CONVERT, HEX2DEC, BITAND, etc.)

### Next Phase 9: Lookup & Reference Functions (Priority: High)
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

### Phase 10: Statistical & Data Analysis Functions (Priority: Medium)
- [ ] **CORRELATION** - Returns the correlation coefficient
- [ ] **COVARIANCE** - Returns covariance
- [ ] **PERCENTILE** - Returns the k-th percentile
- [ ] **QUARTILE** - Returns the quartile value
- [ ] **RANK** - Returns the rank of a number
- [ ] **SLOPE** - Returns the slope of a linear regression line
- [ ] **INTERCEPT** - Returns the y-intercept of a linear regression line
- [ ] **RSQ** - Returns the square of correlation coefficient
- [ ] **FORECAST** - Calculates future value using linear trend
- [ ] **TREND** - Returns values along a linear trend

### ✅ Phase 11: Additional Math Functions (Completed)
- [x] **GCD** - Greatest common divisor
- [x] **LCM** - Least common multiple
- [x] **FACT** - Factorial
- [x] **COMBIN** - Number of combinations
- [x] **PERMUT** - Number of permutations
- [x] **SUMPRODUCT** - Sum of products of arrays
- [x] **SUMIF** - Sum cells that meet a criterion
- [x] **SUMIFS** - Sum cells that meet multiple criteria
- [x] **AVERAGEIF** - Average cells that meet a criterion
- [x] **AVERAGEIFS** - Average cells that meet multiple criteria

### Phase 12: Binary Size Optimization (Priority: High)

**Size Breakdown**:
- COUNTIF function: 146K (16% - regex bloat)
- Core evaluator: 49K (5%)
- Parser: 31K (3%)
- Engineering functions: 30K (3%)
- Other functions: ~660K (72%)

**Optimization Tasks**:
- [ ] **Replace COUNTIF regex usage** - Custom string matching to save ~100K (16% reduction)
- [ ] **Enable Link Time Optimization (LTO)** - Compiler optimization for 10-15% size reduction
- [ ] **Optimize string literals** - Reduce error message and constant string overhead
- [ ] **Template instantiation analysis** - Identify and consolidate redundant template code
- [ ] **Strip unused STL symbols** - Remove unnecessary standard library components
- [ ] **Custom allocators** - Replace STL allocators for smaller footprint
- [ ] **Function inlining review** - Balance performance vs size for hot functions

**Size Analysis Tools**:
```bash
# Check library sizes
ls -lh build/libxl-formula*.a

# Analyze object file sizes
cd build && mkdir temp && cd temp
ar -x ../libxl-formula.a && ls -lhS *.o | head -10

# Check for regex usage (major bloat source)
grep -r "#include <regex>" cpp/functions/
```

### Phase 13: Platform Bindings (Priority: High)
- [ ] **React Native Bindings** - Nitro modules for mobile apps
- [ ] **WASM/Emscripten Bindings** - Web browser compatibility
- [ ] **NPM Package Distribution** - Unified package for all platforms
- [ ] **TypeScript Definitions** - Type safety for JavaScript/TypeScript
- [ ] **Documentation Website** - Interactive examples and API docs

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