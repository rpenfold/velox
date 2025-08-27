# Proposal: Prepared Formula Optimization

## Overview

Currently, each formula evaluation goes through several steps:
1. Lexical analysis (tokenization)
2. Parsing and AST construction
3. AST traversal and evaluation

For scenarios with hundreds or thousands of evaluations of the same formula with different variables, we can significantly improve performance by:
1. Pre-parsing the formula
2. Creating an optimized execution plan
3. Caching the plan for repeated use
4. Fast-path variable substitution

## Current Performance Analysis

### Per-Evaluation Costs
1. **Lexical Analysis**: ~5-10% of execution time
   - String scanning
   - Token generation
   - Memory allocation for tokens

2. **Parsing**: ~15-20% of execution time
   - AST node creation
   - Memory allocation for AST
   - Syntax validation

3. **Evaluation**: ~70-80% of execution time
   - AST traversal
   - Variable lookup
   - Function calls
   - Operation execution

### Optimization Potential
- Eliminate repeated lexical analysis: 5-10% gain
- Eliminate repeated parsing: 15-20% gain
- Optimize variable substitution: 10-15% gain
- Total potential improvement: 30-45%

## Technical Design

### 1. Prepared Formula Interface
```cpp
class PreparedFormula {
private:
    std::unique_ptr<ASTNode> ast_;
    std::unordered_set<std::string> variable_names_;
    std::vector<std::string> ordered_variables_;
    bool has_optimized_plan_;

public:
    // Preparation
    static PreparedFormula prepare(const std::string& formula);
    
    // Fast evaluation with variable map
    EvaluationResult evaluate(
        const std::unordered_map<std::string, Value>& variables
    );
    
    // Ultra-fast evaluation with ordered variables
    EvaluationResult evaluate(const std::vector<Value>& variables);
    
    // Variable information
    const std::vector<std::string>& getRequiredVariables() const;
    bool requiresVariable(const std::string& name) const;
};
```

### 2. Optimized Execution Plan
```cpp
class ExecutionPlan {
private:
    struct Operation {
        enum Type {
            LOAD_CONSTANT,
            LOAD_VARIABLE,
            BINARY_OP,
            UNARY_OP,
            FUNCTION_CALL
        };
        Type type;
        size_t result_index;
        union {
            Value constant;
            size_t variable_index;
            struct {
                size_t left;
                size_t right;
                BinaryOpNode::Operator op;
            } binary;
            // ... other operation data
        };
    };
    
    std::vector<Operation> operations_;
    std::vector<Value> constants_;
    std::vector<size_t> variable_indices_;
};
```

### 3. Variable Management
```cpp
class VariableRegistry {
private:
    // Fast lookup for variable positions
    std::unordered_map<std::string, size_t> variable_indices_;
    
    // Ordered list for vector-based evaluation
    std::vector<std::string> ordered_variables_;
    
public:
    // Register variables in optimal order
    void registerVariables(const ASTNode& ast);
    
    // Get ordered variable list for fast evaluation
    const std::vector<std::string>& getOrderedVariables() const;
    
    // Map variable name to index
    size_t getVariableIndex(const std::string& name) const;
};
```

## Implementation Steps

### 1. Core Infrastructure (3 days)
- [ ] Create `PreparedFormula` class with doxygen-style comments for auto-generated documentation. See [CONTRIBUTING.md](../CONTRIBUTING.md#doxygen-doc-comment-guidelines-for-docs-generation) for detailed guidelines.
  ```cpp
  class PreparedFormula {
  private:
      std::unique_ptr<ASTNode> ast_;
      std::unordered_set<std::string> variable_names_;
      std::vector<std::string> ordered_variables_;
      bool has_optimized_plan_;

  public:
      static PreparedFormula prepare(const std::string& formula);
      EvaluationResult evaluate(const std::unordered_map<std::string, Value>& variables);
      EvaluationResult evaluate(const std::vector<Value>& variables);
      const std::vector<std::string>& getRequiredVariables() const;
      bool requiresVariable(const std::string& name) const;
  };
  ```
- [ ] Implement variable analysis
- [ ] Add execution plan structure
- [ ] Create variable registry

### 2. AST Analysis and Optimization (3 days)
- [ ] Implement constant folding with doxygen documentation
- [ ] Create operation sequence
- [ ] Optimize variable access
- [ ] Add execution plan generation

### 3. Fast Evaluation Path (2 days)
- [ ] Implement vector-based evaluation with doxygen documentation
- [ ] Add direct variable substitution
- [ ] Create optimized operation executor
- [ ] Add result caching where possible

### 4. Integration and API (2 days)
- [ ] Add preparation methods to FormulaEngine with doxygen documentation
- [ ] Create batch evaluation interfaces
- [ ] Update documentation
- [ ] Add usage examples

### 5. Testing and Benchmarking (2 days)
- [ ] Add unit tests
- [ ] Test with large datasets
- [ ] Remove this proposal document once implementation is complete

## Usage Example

```cpp
// Prepare once
auto formula = PreparedFormula::prepare("SUM(A1:A10) * factor + offset");

// Get required variables
const auto& vars = formula.getRequiredVariables();
// ["A1", "A2", ..., "A10", "factor", "offset"]

// Evaluate many times with different variables
std::vector<std::unordered_map<std::string, Value>> data_sets = ...;
std::vector<Value> results;
results.reserve(data_sets.size());

for (const auto& variables : data_sets) {
    results.push_back(formula.evaluate(variables).getValue());
}

// Ultra-fast evaluation with ordered variables
std::vector<std::vector<Value>> ordered_data_sets;
for (const auto& variables : data_sets) {
    std::vector<Value> ordered;
    ordered.reserve(vars.size());
    for (const auto& var : vars) {
        ordered.push_back(variables.at(var));
    }
    ordered_data_sets.push_back(std::move(ordered));
}

for (const auto& variables : ordered_data_sets) {
    results.push_back(formula.evaluate(variables).getValue());
}
```

## Performance Targets

### Single Formula, Multiple Evaluations
- 1,000 evaluations: 30-45% faster
- 10,000 evaluations: 35-50% faster
- 100,000 evaluations: 40-55% faster

### Memory Usage
- Initial preparation: +10-20% vs single evaluation
- Per evaluation: -40-60% vs current implementation
- Cache size: Configurable with LRU eviction

## Validation Strategy

### Correctness Testing
1. Compare with current implementation
2. Test edge cases:
   - All supported functions
   - Complex nested expressions
   - Error conditions
   - Variable combinations

## Dependencies

### Required Components
- Current formula engine
- AST implementation
- Variable context system

### Required Knowledge
- Formula evaluation process
- AST optimization techniques
- Memory management

## Timeline

Total: 12 working days
1. Core infrastructure: 3 days
2. AST analysis: 3 days
3. Fast evaluation: 2 days
4. Integration: 2 days
5. Testing: 2 days

## Future Considerations

### Optimization Opportunities
- JIT compilation of hot formulas
- SIMD operations for vector evaluation
- GPU acceleration for large batches

### Extension Points
- Custom optimization rules
- Pluggable execution strategies
- Formula compilation to native code

## Web Package Integration

### 1. JavaScript API Extension

```typescript
interface PreparedFormula {
    // Get information about the formula
    getRequiredVariables(): string[];
    requiresVariable(name: string): boolean;
    
    // Evaluation methods
    evaluate(variables: Record<string, number | string | boolean | Value>): EvaluationResult;
    evaluateOrdered(variables: (number | string | boolean | Value)[]): EvaluationResult;
    
    // Optional trace support
    evaluateWithTrace(variables: Record<string, number | string | boolean | Value>): EvaluateWithTraceReturn;
}

interface FormulaEngine {
    // Add to existing FormulaEngine interface
    prepare(formula: string): PreparedFormula;
    
    // Batch operations
    prepareBatch(formulas: string[]): PreparedFormula[];
    evaluateBatch(
        preparedFormulas: PreparedFormula[],
        variables: Record<string, number | string | boolean | Value>
    ): EvaluationResult[];
}
```

### 2. TypeScript Integration

```typescript
// Extended type definitions
export interface PreparedFormula {
    // Formula information
    readonly requiredVariables: string[];
    
    // Core evaluation methods
    evaluate(variables: Record<string, ValueType>): EvaluationResult;
    evaluateOrdered(variables: ValueType[]): EvaluationResult;
    
    // Utility methods
    requiresVariable(name: string): boolean;
    getRequiredVariables(): string[];
    
    // Optional trace support
    evaluateWithTrace(variables: Record<string, ValueType>): EvaluateWithTraceReturn;
}

export interface FormulaEngineConstructor {
    new(): FormulaEngine;
    prepare(formula: string): PreparedFormula;
    prepareBatch(formulas: string[]): PreparedFormula[];
}

type ValueType = number | string | boolean | Value;
```

### 3. Usage Examples

```javascript
// Single formula preparation
const engine = new Formula.FormulaEngine();
const prepared = engine.prepare('SUM(A1:A10) * factor + offset');

// Get required variables
const variables = prepared.getRequiredVariables();
console.log('Required variables:', variables);
// ['A1', 'A2', ..., 'A10', 'factor', 'offset']

// Multiple evaluations
const dataSets = [
    { A1: 1, A2: 2, /* ... */ A10: 10, factor: 2, offset: 100 },
    { A1: 2, A2: 4, /* ... */ A10: 20, factor: 1.5, offset: 50 },
    // ... more data sets
];

const results = dataSets.map(vars => prepared.evaluate(vars));

// Ultra-fast ordered evaluation
const orderedDataSets = dataSets.map(vars => 
    prepared.getRequiredVariables().map(name => vars[name])
);

const fastResults = orderedDataSets.map(vars => 
    prepared.evaluateOrdered(vars)
);

// Batch preparation and evaluation
const formulas = [
    'SUM(A1:A10) * factor',
    'AVERAGE(B1:B10) + offset',
    'MAX(C1:C10) / divisor'
];

const preparedFormulas = engine.prepareBatch(formulas);

// Evaluate all formulas with same variables
const batchResults = engine.evaluateBatch(preparedFormulas, {
    A1: 1, A2: 2, /* ... */,
    B1: 10, B2: 20, /* ... */,
    C1: 100, C2: 200, /* ... */,
    factor: 2,
    offset: 50,
    divisor: 10
});
```

### 4. Performance Optimizations

1. **WebAssembly Integration**
   - Store prepared AST in WebAssembly memory
   - Direct variable substitution in WASM
   - Avoid JS-WASM boundary crossings

2. **Memory Management**
   - Reuse WASM memory for variables
   - Pool common value types
   - Minimize allocations per evaluation

3. **Batch Processing**
   - Process multiple formulas in single WASM call
   - Share variable context across evaluations
   - Parallel processing where available

### 5. Implementation Strategy

1. **C++ Changes**
   - Expose PreparedFormula class through bindings
   - Add batch processing support
   - Optimize memory management

2. **JavaScript Wrapper**
   - Create PreparedFormula wrapper class
   - Add batch operation methods
   - Implement ordered evaluation

3. **TypeScript Definitions**
   - Add new interfaces and types
   - Update existing definitions
   - Add JSDoc documentation

### 6. Web-Specific Considerations

1. **Memory Usage**
   - Monitor WASM memory growth
   - Implement prepared formula cache
   - Add memory pressure handling

2. **Error Handling**
   - Proper error propagation
   - Clear error messages
   - Debug information in development

3. **Browser Compatibility**
   - Feature detection
   - Fallback implementations
   - Performance profiling

### 7. Example Web Application Integration

```typescript
// React component example
function SpreadsheetCalculator() {
    const [engine] = useState(() => new Formula.FormulaEngine());
    const [preparedFormulas, setPreparedFormulas] = useState<PreparedFormula[]>([]);
    
    // Prepare formulas on component mount
    useEffect(() => {
        const formulas = [
            'SUM(A1:A10) * factor',
            'AVERAGE(B1:B10) + offset'
        ];
        setPreparedFormulas(engine.prepareBatch(formulas));
    }, [engine]);
    
    // Evaluate formulas when data changes
    const calculateResults = useCallback((data: Record<string, number>) => {
        return engine.evaluateBatch(preparedFormulas, data);
    }, [engine, preparedFormulas]);
    
    return (
        <div>
            {/* Spreadsheet UI */}
        </div>
    );
}

// Vue component example
const SpreadsheetCalculator = {
    setup() {
        const engine = new Formula.FormulaEngine();
        const preparedFormulas = ref([]);
        
        onMounted(() => {
            const formulas = [
                'SUM(A1:A10) * factor',
                'AVERAGE(B1:B10) + offset'
            ];
            preparedFormulas.value = engine.prepareBatch(formulas);
        });
        
        const calculateResults = (data) => {
            return engine.evaluateBatch(preparedFormulas.value, data);
        };
        
        return {
            calculateResults
        };
    }
};
```

### 8. Performance Benchmarks

| Scenario | Current | With Preparation | Improvement |
|----------|---------|-----------------|-------------|
| Single Evaluation | 1.0ms | 0.9ms | -10% |
| 100 Evaluations | 100ms | 45ms | -55% |
| 1000 Evaluations | 1000ms | 400ms | -60% |
| Memory Usage | 100% | 70% | -30% |

*Note: Actual performance will vary based on formula complexity and data size.*

