# Velox Code Organization

This document gives a concise overview of how the Velox codebase is structured and where to add or modify functionality.

## High-level layout

```
cpp/
├── core/           # Value types, Context, public API glue
├── parser/         # Lexer, parser, AST
├── engine/         # Evaluator, FormulaEngine
├── functions/      # Built-in function implementations by domain
│   ├── math/
│   ├── text/
│   ├── logical/
│   ├── datetime/
│   ├── financial/
│   ├── engineering/
│   └── utils/
└── bindings/
    └── web/       # Emscripten bindings (WASM)

packages/web/       # JS distribution (@velox/formulas)
apps/docs/          # Documentation and playground site
```

## Design principles

- Single responsibility: one concept per file
- Clear boundaries: parsing (parser), evaluation (engine), rendering/bindings (bindings)
- Domain grouping: functions live under a domain directory; registration is centralized
- Small compilation units: faster builds and easier debugging

## Adding a function (overview)

1. Implement the function in the appropriate domain directory under `cpp/functions/<domain>/`.
2. Add a forward declaration if needed in public headers under `cpp/include/xl-formula/`.
3. Register the function name in the dispatcher (see `cpp/functions/fn_dispatcher.cpp`).
4. Add tests in `tests/functions/<domain>/`.
5. Add minimal docs (examples and short description) to the docs data if applicable.

See `docs/CONTRIBUTING.md` for step-by-step details and testing guidance.

## Notes on bindings and JS package

- Web bindings are generated via Emscripten in `cpp/bindings/web/` and surfaced through `packages/web/` as `@velox/formulas`.
- The docs app (`apps/docs/`) consumes `@velox/formulas` directly via workspace aliasing in development.

## Future extensibility

- New domains can be introduced by adding a top-level folder under `cpp/functions/` and wiring it into the dispatcher.
- Platform-specific bindings can live under `cpp/bindings/<platform>/` without impacting the core.
