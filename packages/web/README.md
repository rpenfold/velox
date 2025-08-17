# Velox Formulas (Web)

High-performance Excel-like formula parser and evaluator for web applications, compiled to WebAssembly.

## Features

- **Fast**: Compiled to WebAssembly for near-native performance
- **Complete**: Supports 100+ Excel functions across Math, Text, Logical, Date/Time, Financial, and Engineering categories  
- **Excel Compatible**: Supports both Excel-style formulas with '=' prefix and direct expressions
- **Type Safe**: Full TypeScript definitions included
- **Easy to Use**: Simple JavaScript API with both standalone functions and stateful engine
- **Zero Dependencies**: Self-contained WebAssembly module
- **Modern**: ES6 modules with fallback to CommonJS

## Quick Start

### Installation

```bash
npm install @velox/formulas
```

### Basic Usage

```javascript
import XLFormula from '@velox/formulas';

// Initialize the library
await XLFormula.init();

// Simple evaluation (supports both Excel-style '=' prefix and direct input)
const result = XLFormula.evaluate('=SUM(1, 2, 3, 4, 5)');
if (result.isSuccess()) {
  console.log(result.getValue().asNumber()); // 15
}

// Using the engine for stateful evaluation
const engine = new XLFormula.FormulaEngine();
engine.setNumber('A1', 10);
engine.setNumber('B1', 20);
engine.setText('C1', 'Hello');

const sumResult = engine.evaluate('=A1 + B1');
const sum = sumResult.isSuccess() ? sumResult.getValue().asNumber() : 0; // 30
const concatResult = engine.evaluate('=C1 & " World"');
const concat = concatResult.isSuccess() ? concatResult.getValue().asText() : ''; // "Hello World"
```

### Advanced Usage

```javascript
import XLFormula from '@velox/formulas';

await XLFormula.init();

const engine = new XLFormula.FormulaEngine();

// Set variables of different types
engine.setNumber('price', 100);
engine.setNumber('taxRate', 0.08);
engine.setBoolean('isTaxable', true);

// Complex formula with conditionals (Excel-style with '=' or direct input)
const result = engine.evaluate('=IF(isTaxable, price * (1 + taxRate), price)');

if (result.isSuccess()) {
    console.log('Total price:', result.getValue().asNumber()); // 108
    console.log('Value type:', result.getValue().getTypeName()); // "number"
} else {
    console.error('Error:', result.getErrorMessage());
}
```

## Supported Functions

### Math Functions
- Basic: `SUM`, `MAX`, `MIN`, `AVERAGE`, `COUNT`, `ABS`, `ROUND`, `TRUNC`
- Trigonometry: `SIN`, `COS`, `TAN`, `ASIN`, `ACOS`, `ATAN`, `ATAN2`
- Advanced: `SQRT`, `POWER`, `EXP`, `LN`, `LOG`, `LOG10`, `PI`
- Statistics: `STDEV`, `VAR`, `MEDIAN`, `MODE`

### Text Functions
- `CONCATENATE`, `LEN`, `LEFT`, `RIGHT`, `MID`
- `UPPER`, `LOWER`, `PROPER`, `TRIM`
- `FIND`, `SEARCH`, `REPLACE`, `SUBSTITUTE`
- `TEXT`, `VALUE`

### Logical Functions
- `IF`, `IFERROR`, `IFNA`
- `AND`, `OR`, `NOT`, `XOR`
- `TRUE`, `FALSE`
- Type checking: `ISNUMBER`, `ISTEXT`, `ISBLANK`, `ISERROR`

### Date/Time Functions
- `NOW`, `TODAY`, `DATE`, `TIME`
- `YEAR`, `MONTH`, `DAY`, `HOUR`, `MINUTE`, `SECOND`
- `WEEKDAY`, `DATEDIF`

### Financial Functions
- `PV`, `FV`, `PMT`, `RATE`, `NPER`
- `NPV`, `IRR`, `MIRR`

### Engineering Functions
- `BIN2DEC`, `DEC2BIN`, `DEC2HEX`, `HEX2DEC`
- `BITAND`, `BITOR`, `BITXOR`
- `CONVERT`

## API Reference

### Static Methods

- `XLFormula.init()`: Initialize the WebAssembly module (returns Promise<boolean>)
- `XLFormula.isInitialized()`: Check if the library is ready
- `XLFormula.evaluate(formula)`: Quick evaluation returning EvaluationResult
- `XLFormula.evaluate(formula)`: Quick evaluation (returns EvaluationResult)
- `XLFormula.getVersion()`: Get library version

### FormulaEngine Class

```javascript
const engine = new XLFormula.FormulaEngine();

// Variable management
engine.setVariable(name, value);  // Auto-detect type
engine.setNumber(name, number);
engine.setText(name, string);
engine.setBoolean(name, boolean);
engine.getVariable(name);
engine.hasVariable(name);
engine.removeVariable(name);
engine.clearVariables();

// Evaluation
engine.evaluate(formula);        // Returns EvaluationResult
const result = engine.evaluate(formula);
if (result.isSuccess()) {
  const value = result.getValue();
  if (value.isNumber()) {
    console.log(value.asNumber());  // Returns number
  } else if (value.isText()) {
    console.log(value.asText());    // Returns string
  } else if (value.isBoolean()) {
    console.log(value.asBoolean()); // Returns boolean
  }
}
```

### Value Class

```javascript
const value = result.getValue();

// Type checking
value.isNumber();
value.isText();
value.isBoolean();
value.isError();
value.isEmpty();
value.isDate();

// Value extraction
value.asNumber();
value.asText();
value.asBoolean();
value.getErrorText();
value.getTypeName();
```

### EvaluationResult Class

```javascript
const result = engine.evaluate(formula);

result.isSuccess();        // boolean
result.hasError();         // boolean  
result.getValue();         // Value object
result.getErrorMessage();  // string
result.getErrors();        // string[]
```

## Browser Compatibility

- Chrome 57+
- Firefox 52+
- Safari 11+
- Edge 16+

Requires WebAssembly support.

## Demo

A live demo is included in the package. After installation, serve the files:

```bash
cd node_modules/@velox/formulas
python3 -m http.server 8000
# Open http://localhost:8000 in your browser
```

## Building from Source

```bash
git clone https://github.com/your-username/velox.git
cd velox
./build.sh --web
```

Requires Emscripten SDK to be installed.

## License

MIT License - see LICENSE file for details.