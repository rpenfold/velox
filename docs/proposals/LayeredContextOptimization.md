# Performance Optimization: Layered Context for Variable Map Evaluation

## Current Implementation

The current implementation of `evaluate()` with a variables map has overhead from:
1. Snapshotting original values
2. Applying overrides to context
3. Evaluating formula
4. Restoring original context

This approach requires:
- Memory allocation for storing original values
- Multiple map operations for saving/restoring state
- Modification of the engine's context

## Proposed Optimization

Replace the copy-restore approach with a layered context lookup that references the override map directly.

### Key Goals

1. **Zero Impact on Standard Path**
   - The regular `evaluate()` without variables map MUST NOT have any performance impact
   - No virtual dispatch or indirection in the common case
   - No memory layout changes that could affect cache behavior
   - No additional branches in hot paths

2. **Memory Efficiency**
   - Eliminate variable copying
   - Avoid temporary allocations
   - Maintain small memory footprint

3. **Clean Design**
   - Clear separation between base and override contexts
   - Thread-safe by design
   - Easy to reason about variable precedence

### Implementation Strategy

```cpp
class LayeredContext : public Context {
private:
    const std::unordered_map<std::string, Value>* override_variables_;
    const Context* base_context_;

public:
    LayeredContext(const std::unordered_map<std::string, Value>* overrides, 
                  const Context* base)
        : override_variables_(overrides), base_context_(base) {}

    Value getVariable(const std::string& name) const override {
        // Check overrides first
        if (override_variables_) {
            auto it = override_variables_->find(name);
            if (it != override_variables_->end()) {
                return it->second;
            }
        }
        
        // Fall back to base context
        if (base_context_) {
            return base_context_->getVariable(name);
        }
        
        return Value::empty();
    }
};
```

**Note:** All classes and methods should be annotated with doxygen-style comments for auto-generated documentation. See [CONTRIBUTING.md](../CONTRIBUTING.md#doxygen-doc-comment-guidelines-for-docs-generation) for detailed guidelines.

### Required Benchmarking

Before implementing this optimization, we need:

1. **Baseline Measurements**
   - Performance of current `evaluate()` without variables
   - Performance of current `evaluate()` with variables map
   - Memory allocation patterns and sizes
   - Cache behavior analysis

2. **Test Scenarios**
   - Small vs large number of variables
   - Different ratios of override vs base variables
   - Repeated evaluations with same/different overrides
   - Impact of virtual dispatch on hot paths
   - Cache miss rates with layered lookup

3. **Verification Tests**
   - No regression in standard `evaluate()`
   - Correct precedence of variable lookup
   - Thread safety under concurrent access
   - Memory usage patterns
   - Error handling and edge cases

### Implementation Phases

1. **Phase 1: Benchmarking Infrastructure**
   - Set up comprehensive benchmark suite
   - Establish baseline metrics
   - Define success criteria

2. **Phase 2: Prototype**
   - Implement LayeredContext
   - Add necessary test coverage
   - Measure initial performance impact

3. **Phase 3: Optimization**
   - Profile and optimize based on benchmarks
   - Ensure zero impact on standard path
   - Fine-tune memory behavior

4. **Phase 4: Integration**
   - Careful integration with existing codebase
   - Comprehensive testing
   - Remove this proposal document once implementation is complete

### Risk Analysis

1. **Performance Risks**
   - Virtual dispatch overhead
   - Cache locality impact
   - Memory access patterns
   - Branch prediction effects

2. **Implementation Risks**
   - Thread safety considerations
   - Lifetime management of override map
   - Error propagation
   - Edge cases in variable lookup

### Success Criteria

1. **Performance**
   - Zero measurable impact on standard `evaluate()`
   - At least 20% improvement for variables map case
   - No memory allocation in hot path
   - Better cache behavior than current implementation

2. **Quality**
   - All existing tests pass
   - No new memory leaks
   - Thread safety maintained
   - Clean error handling

### Future Considerations

1. **Potential Enhancements**
   - Multiple layers of overrides
   - Cached lookup paths
   - Compile-time optimization hints
   - Integration with FastEvaluate proposal

2. **Integration Points**
   - Formula compilation
   - JIT optimization
   - Parallel evaluation
   - Variable scope management

## Next Steps

1. Implement benchmark tooling as outlined in separate proposal
2. Establish baseline metrics for current implementation
3. Create prototype implementation
4. Validate performance characteristics
5. Make go/no-go decision based on data

## Dependencies

- Comprehensive benchmark suite
- Performance profiling tools
- Thread safety analysis tools
- Memory profiling capabilities
