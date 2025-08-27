# Velox

A collection of high-performance libraries for modern web applications, built with WebAssembly and modern web technologies to deliver exceptional speed and reliability.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/your-org/xl-formula)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Coverage](https://img.shields.io/badge/coverage-95%25+-green.svg)](#testing)

## Overview

Velox is designed to bring native-level performance to web applications through a collection of specialized libraries. Whether you're building data-intensive applications, real-time dashboards, or complex computational tools, Velox provides the performance foundation you need.

## Libraries

### 🧮 Velox Formulas
High-performance Excel-like formula parsing and evaluation library implemented in C++ with WebAssembly bindings.

**Key Features:**
- **85+ Excel-compatible functions** across 8 categories
- **WebAssembly-powered performance** for complex calculations
- **TypeScript support** with comprehensive type definitions
- **Zero dependencies** and easy integration

[📖 Documentation](./apps/docs/src/pages/FormulasPage.jsx) | [🎮 Playground](./apps/docs/src/pages/PlaygroundPage.jsx) | [📦 npm package](https://www.npmjs.com/package/@velox/formulas)

### 🚀 More Libraries Coming Soon
We're working on additional high-performance libraries for data processing, visualization, and more. Stay tuned for updates!

## Quick Start

### Velox Formulas

```bash
npm install @velox/formulas
```

```javascript
import XLFormula from '@velox/formulas'

await XLFormula.init()
const result = XLFormula.evaluate('SUM(1,2,3)')
if (result.isSuccess()) {
  console.log(result.getValue().asNumber()) // 6
}
```

### 📱 Mobile Development (React Native)

Velox includes a React Native app for testing and experimenting with the formula engine in a mobile environment.

#### Prerequisites

- **Node.js 18+** and npm
- **Xcode 15+** (for iOS development)
- **Android Studio** (for Android development)
- **CocoaPods** (for iOS dependencies)

#### Quick Start

For a newly cloned repository, the setup is simple:

```bash
# 1. Install dependencies and apply patches
npm ci

# 2. Install iOS pods
npm run mobile:prepare

# 3. Start Metro bundler
npm run mobile:start

# 4. Run on iOS simulator
npm run mobile:ios

# Or run on Android emulator
npm run mobile:android
```

#### Available Scripts

From the project root, you can use these convenient scripts:

- **`npm run mobile:setup`** - Complete setup (npm ci + pod install)
- **`npm run mobile:prepare`** - Install iOS CocoaPods
- **`npm run mobile:start`** - Start Metro bundler on port 8082
- **`npm run mobile:ios`** - Build and run on iOS simulator
- **`npm run mobile:android`** - Build and run on Android emulator
- **`npm run mobile:dev`** - Start Metro and run iOS app simultaneously
- **`npm run mobile:clean`** - Clean build artifacts and reinstall pods

#### Development Workflow

1. **Start Metro bundler**: `npm run mobile:start`
2. **Make changes** to your React Native code
3. **Hot reload** automatically in the simulator
4. **Build and run**: `npm run mobile:ios` or `npm run mobile:android`

#### Troubleshooting

If you encounter issues:

```bash
# Clean everything and start fresh
npm run mobile:clean
npm run mobile:setup
npm run mobile:start
```
```

## Features

### 🚀 High Performance
- Written in modern C++17 for optimal speed and memory efficiency
- **WebAssembly compilation** for near-native browser performance
- **Perfect hash function dispatcher** for ultra-fast function calls (6-91% faster)
- Decoupled parser and evaluator architecture
- Optimized for repeated evaluations with variable contexts

### 🔧 Developer Experience
- **TypeScript support** with comprehensive type definitions
- **Comprehensive documentation** and interactive playgrounds
- **Easy integration** with zero dependencies
- **Cross-platform compatibility** (Web, React Native, Native)

### 🔒 Production Ready
- **Battle-tested libraries** with comprehensive test coverage
- **Active maintenance** and regular updates
- **Comprehensive error handling** with detailed error messages
- **Performance benchmarks** and optimization

## Supported Functions (Velox Formulas)

Currently supports **85+ built-in functions** across 8 categories:

#### 📊 Math & Statistical Functions (34)
- **Basic**: `SUM`, `MIN`, `MAX`, `AVERAGE`, `COUNT`, `COUNTA`
- **Rounding**: `ABS`, `ROUND`, `CEILING`, `FLOOR`, `INT`, `TRUNC`, `SIGN`
- **Advanced**: `SQRT`, `POWER`, `MOD`, `PI`, `RAND`, `RANDBETWEEN`
- **Statistical**: `MEDIAN`, `MODE`, `STDEV`, `VAR`, `COUNTIF`
- **Conditional**: `SUMIF`, `SUMIFS`, `AVERAGEIF`, `AVERAGEIFS`
- **Combinatorics**: `GCD`, `LCM`, `FACT`, `COMBIN`, `PERMUT`
- **Arrays**: `SUMPRODUCT`

#### 📝 Text Functions (14) 
- **Manipulation**: `CONCATENATE`, `TRIM`, `LEN`, `LEFT`, `RIGHT`, `MID`
- **Formatting**: `UPPER`, `LOWER`, `PROPER`, `TEXT`, `VALUE`
- **Search**: `FIND`, `SEARCH`, `REPLACE`, `SUBSTITUTE`

#### 🔄 Logical Functions (14)
- **Basic**: `TRUE`, `FALSE`, `IF`, `AND`, `OR`, `NOT`, `XOR`
- **Error Handling**: `IFERROR`, `IFNA`, `ISERROR`
- **Type Testing**: `ISNUMBER`, `ISTEXT`, `ISBLANK`

#### 📅 Date & Time Functions (12)
- **Current**: `NOW`, `TODAY`
- **Construction**: `DATE`, `TIME`
- **Extraction**: `YEAR`, `MONTH`, `DAY`, `HOUR`, `MINUTE`, `SECOND`
- **Calculations**: `WEEKDAY`, `DATEDIF`

#### 🔢 Trigonometric Functions (16)
- **Basic**: `SIN`, `COS`, `TAN`, `ASIN`, `ACOS`, `ATAN`, `ATAN2`
- **Hyperbolic**: `SINH`, `COSH`, `TANH`
- **Conversion**: `DEGREES`, `RADIANS`
- **Logarithmic**: `EXP`, `LN`, `LOG`, `LOG10`

#### 💰 Financial Functions (8)
- **Time Value**: `PV`, `FV`, `PMT`, `RATE`, `NPER`
- **Analysis**: `NPV`, `IRR`, `MIRR`

#### ⚙️ Engineering Functions (8)
- **Conversion**: `CONVERT`, `HEX2DEC`, `DEC2HEX`, `BIN2DEC`, `DEC2BIN`
- **Bitwise**: `BITAND`, `BITOR`, `BITXOR`

### Supported Operations

- **Arithmetic**: `+`, `-`, `*`, `/`, `^` (power)
- **Comparison**: `=`, `<>`, `<`, `<=`, `>`, `>=`
- **Text**: `&` (concatenation)
- **Unary**: `+`, `-`
- **Grouping**: `(` `)`

## Building

### Prerequisites

- CMake 3.20+
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Node.js 16+ (for web builds and documentation)
- Optional: Emscripten (for web builds)

### Quick Build

```bash
# Basic build
./build.sh

# Debug build with tests
./build.sh --debug --tests

# Coverage build
./build.sh --coverage --tests

# Format all code
./build.sh --format

# Check formatting (CI-friendly)
./build.sh --format-check
```

### CLI (formula)

Run the following to add the CLI to your path

```bash
echo "export PATH=\"$PWD/scripts:$PATH\"" >> ~/.zshrc && source ~/.zshrc
```

Then you can run `formula [-h|--help|help]` for details on how to use the CLI.

### Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Run tests
ctest

# Run examples
./examples/basic_example
./examples/advanced_example
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `BUILD_TESTS` | Build test suite | ON |
| `BUILD_EXAMPLES` | Build example programs | ON |
| `BUILD_WEB_BINDINGS` | Build web bindings | ON |
| `BUILD_RN_BINDINGS` | Build React Native bindings | OFF |

## Project Structure

```
xl-formula/
├── apps/
│   └── docs/              # Documentation website
├── packages/
│   └── web/               # Web package distribution
├── cpp/                   # Core C++ implementation
│   ├── core/              # Value types, Context, API
│   ├── parser/            # Lexer, Parser, AST
│   ├── engine/            # Evaluator, FormulaEngine
│   └── functions/         # Built-in functions
│       ├── math/          # Mathematical functions
│       ├── text/          # Text manipulation functions
│       ├── logical/       # Logical functions
│       └── utils/         # Utility functions
├── cpp/include/xl-formula/    # Public headers
├── tests/                 # Comprehensive test suite
├── examples/              # Usage examples
└── scripts/               # Build and utility scripts
```

## API Reference

### Core Classes

#### `FormulaEngine`
Main interface for formula evaluation.

```cpp
class FormulaEngine {
public:
    EvaluationResult evaluate(const std::string& formula);
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name) const;
    void registerFunction(const std::string& name, const FunctionImpl& impl);
};
```

#### `Value`
Represents a value in the formula system.

```cpp
class Value {
public:
    Value(double number);
    Value(const std::string& text);
    Value(bool boolean);
    
    bool isNumber() const;
    bool isText() const;
    bool isBoolean() const;
    bool isError() const;
    
    double asNumber() const;
    std::string asText() const;
    bool asBoolean() const;
    std::string toString() const;
};
```

### Custom Functions

Register custom functions to extend functionality:

```cpp
engine.registerFunction("DOUBLE", [](const std::vector<xl_formula::Value>& args, const xl_formula::Context& ctx) {
    if (args.size() != 1) return xl_formula::Value::error(xl_formula::ErrorType::VALUE_ERROR);
    if (!args[0].canConvertToNumber()) return xl_formula::Value::error(xl_formula::ErrorType::VALUE_ERROR);
    return xl_formula::Value(args[0].toNumber() * 2);
});

auto result = engine.evaluate("DOUBLE(21)"); // Returns 42
```

## Web Usage

After building with web bindings:

```javascript
import { XLFormula } from '@velox/formulas';

const engine = new XLFormula();
engine.setVariable('A1', 10);
engine.setVariable('A2', 20);

const result = engine.evaluate('SUM(A1, A2)');
console.log(result); // 30
```

## React Native Usage

```typescript
import { XLFormula } from '@velox/formulas';

const engine = new XLFormula();
engine.setVariable('sales', 1000);
engine.setVariable('tax_rate', 0.08);

const total = engine.evaluate('sales * (1 + tax_rate)');
console.log(total); // 1080
```

## Testing

The library includes comprehensive tests with >95% code coverage:

```bash
# Run all tests
./build.sh --tests

# Run specific test category
./build/xl-formula-tests --gtest_filter="*Parser*"

# Generate coverage report
./build.sh --coverage --tests
```

### Test Categories

- **Unit Tests**: Individual component testing
- **Integration Tests**: End-to-end workflow testing
- **Performance Tests**: Baseline performance verification
- **Error Handling Tests**: Comprehensive error condition coverage

## Examples

### Basic Example
Simple arithmetic and function usage:

```bash
./build/examples/basic_example
```

### Advanced Example
Simulates a spreadsheet with custom functions and complex formulas:

```bash
./build/examples/advanced_example
```

### Real-World Use Cases

1. **Financial Calculations**
   ```cpp
   engine.setVariable("principal", Value(10000.0));
   engine.setVariable("rate", Value(0.05));
   engine.setVariable("years", Value(10.0));
   
   auto compound_interest = engine.evaluate("principal * ((1 + rate) ^ years)");
   ```

2. **Data Validation**
   ```cpp
   auto validation = engine.evaluate(
       "IF(AND(age >= 18, income > 30000), \"Approved\", \"Denied\")"
   );
   ```

3. **Report Generation**
   ```cpp
   auto report = engine.evaluate(
       "\"Sales Report: \" & SUM(Q1, Q2, Q3, Q4) & \" total revenue\""
   );
   ```

4. **Logical Operations**
   ```cpp
   // Complex conditional logic
   auto result = engine.evaluate(
       "IF(AND(ISNUMBER(A1), A1 > 0), A1 * 1.1, IFERROR(A1, 0))"
   );
   
   // Error handling with fallbacks
   auto safe_division = engine.evaluate(
       "IFERROR(A1 / A2, \"Division by zero\")"
   );
   
   // Type checking
   auto validation = engine.evaluate(
       "IF(OR(ISBLANK(A1), ISTEXT(A1)), \"Invalid input\", A1)"
   );
   ```

## Error Handling

The library provides comprehensive error handling:

- `#DIV/0!` - Division by zero
- `#VALUE!` - Invalid value type
- `#NAME?` - Unknown function or variable
- `#REF!` - Invalid reference
- `#NUM!` - Invalid number
- `#N/A` - Value not available
- `#PARSE!` - Parse error

```cpp
auto result = engine.evaluate("1/0");
if (!result.isSuccess()) {
    std::cout << "Error: " << result.getValue().toString() << std::endl; // "#DIV/0!"
}
```

## Performance

Benchmarks on modern hardware with **perfect hash optimization**:

- **Function Dispatch**: 6-91% faster than traditional hash maps
- **Parse Time**: ~10μs for typical formulas
- **Evaluation Time**: ~5μs for simple arithmetic  
- **Memory Usage**: <1KB per formula AST
- **Throughput**: >100K evaluations/second

## Roadmap

### 🚀 Upcoming Features
- **Lookup Functions**: VLOOKUP, INDEX, MATCH (Phase 9)
- **Advanced Statistics**: CORRELATION, PERCENTILE, RANK (Phase 10)
- **Extended Math**: SUMIF, AVERAGEIF, SUMPRODUCT (Phase 11)

### 🚀 Binary Size Optimization (Phase 12)
- **COUNTIF Refactor**: Replace regex with custom parsing (~100K savings)
- **Link Time Optimization**: Enable LTO for 10-15% size reduction
- **String Optimization**: Minimize error messages and constants
- **Template Analysis**: Reduce redundant instantiations
- **STL Minimization**: Strip unused standard library symbols

### 📱 Platform Bindings (Phase 13)
- **React Native**: Nitro modules for iOS/Android apps
- **WebAssembly**: Emscripten bindings for browsers
- **NPM Package**: Unified distribution for JavaScript/TypeScript
- **Type Definitions**: Full TypeScript support

### 🌟 Future Libraries
- **Data Processing**: High-performance data transformation and analysis
- **Visualization**: WebAssembly-powered charting and graphics
- **Machine Learning**: Optimized ML algorithms for the web

## Contributing

We welcome contributions! Please see our [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines on:

- **Development Setup**: Required tools and dependencies
- **Building and Testing**: Comprehensive build instructions
- **Code Style**: Formatting rules and IDE integration
- **Function Implementation**: Roadmap and guidelines for adding new functions
- **Submitting Changes**: Pull request process and review guidelines

Quick start for contributors:

```bash
# Install prerequisites (see CONTRIBUTING.md for details)
brew install cmake clang-format  # macOS
# sudo apt-get install cmake clang-format  # Ubuntu

# Clone and build
git clone https://github.com/your-org/xl-formula.git
cd xl-formula
./build.sh --debug --tests
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
