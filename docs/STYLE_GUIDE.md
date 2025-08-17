# Velox Style Guide (Docs and Code)

This guide captures the essentials to keep the Velox codebase and docs consistent. Keep it short, practical, and enforceable.

## Documentation

- Use concise, task-first writing. Prefer examples over prose.
- Use backticks for code identifiers (`FormulaEngine`, `Value`).
- Prefer sentence case for headings (e.g., “Getting started”).
- Keep code blocks minimal and copy-paste ready.

## C++ Code

- C++17 minimum.
- 4 spaces, no tabs. Max line length ~100.
- Names
  - Classes, types: PascalCase (e.g., `FormulaEngine`)
  - Functions: snake_case (e.g., `evaluate_expression`)
  - Variables: snake_case (e.g., `result_value`)
  - Constants, enums: UPPER_SNAKE_CASE
- Include order: system, third-party, project headers.
- Favor early returns; avoid deep nesting.
- Document public APIs with brief Doxygen comments.

## JavaScript/TypeScript

- Target modern ESM.
- Keep examples framework-agnostic when possible.
- Prefer strict types in `.d.ts` and explicit exports.

## Commits & PRs

- Conventional commits (`feat:`, `fix:`, `docs:` ...).
- PR checklist: tests pass, lint/format clean, docs updated when needed.

