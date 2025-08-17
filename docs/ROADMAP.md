# Velox Roadmap

This is a living roadmap focused on high-impact, shippable milestones. Dates are intentionally omitted; priorities reflect current intent.

## Near-term priorities

- Web package hardening
  - Stabilize `@velox/formulas` API surface and types
  - Error messages and type names aligned with docs
  - WASM initialization reliability in SSR and bundlers

- Function coverage expansion
  - Lookup & Reference: VLOOKUP, HLOOKUP, INDEX, MATCH
  - Date utilities: EDATE, EOMONTH, WEEKNUM family
  - Statistical: PERCENTILE, QUARTILE, RANK

- Docs site improvements
  - Clear getting started and API reference
  - Function docs searchable and grouped by category
  - Minimal examples runnable in-place

## Medium-term

- Size and performance
  - Reduce COUNTIF/regex footprint with custom matcher
  - Enable LTO and dead-code elimination for release builds
  - Audit string literals and template instantiations

- React Native bindings (preview)
  - Establish package skeleton `@velox/react-native`
  - Define parity goals with web API

## Long-term

- Extended bindings: Python and Node native
- Advanced functions parity with Excel and Formula.js
- Plugin system for custom functions and data providers

## Stretch ideas

- Vectorized evaluation for arrays/ranges
- Streaming evaluator for long-running sheets
- Deterministic evaluation mode for reproducible builds
