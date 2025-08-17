# NPM Workspace Setup

This document explains the npm workspace configuration for the Velox monorepo.

## Workspace Structure

The project is configured as an npm workspace with:

- **Root package.json**: Defines workspace configuration and shared scripts
- **packages/web**: The distributable npm package
- **apps/docs**: The documentation site that depends on packages/web

## Key Benefits

### 1. **No File Duplication**
- WebAssembly files are only built once in `packages/web/`
- `apps/docs` imports directly from the workspace via symlink
- No need to copy files between directories

### 2. **Simplified Dependency Management**
- Single `npm install` at root installs all workspace dependencies
- Workspace dependencies are automatically linked
- Consistent package versions across the monorepo

### 3. **Streamlined Development**
- Build once, use everywhere
- Changes to `packages/web` are immediately available in `apps/docs`
- No manual file copying or sync steps required

## Workspace Configuration

### Root package.json
```json
{
  "workspaces": [
    "packages/*",
    "apps/*"
  ]
}
```

### Apps Dependencies
```json
{
  "dependencies": {
    "@velox/formulas": "*"  // Workspace dependency
  }
}
```

## Usage Commands

From the root directory:

```bash
# Install all workspace dependencies
npm install

# Build the WebAssembly package
npm run build

# Start docs development server
npm run dev

# Build docs for production  
npm run build:docs

# Preview production docs build
npm run preview:docs
```

## How It Works

1. **Installation**: `npm install` creates symlinks in `apps/docs/node_modules/@velox/formulas` pointing to `packages/web/`

2. **Development**: The docs app imports from `@velox/formulas` which resolves to the local workspace package

3. **Building**: The build process ensures `packages/web` is built first via the postinstall hook

4. **Hot Reloading**: Changes to the C++ code trigger a rebuild, and the docs app automatically picks up the changes via the workspace link

## Dependency Flow

```
C++ Source Code
       ↓
   build.sh --web
       ↓
packages/web/ (WebAssembly + JS wrapper)
       ↓ (workspace symlink)
apps/docs/node_modules/@velox/formulas/
       ↓ (ES6 import)
Documentation Site
```

This setup eliminates the complexity of manual file copying while maintaining clean separation between the distributable package and the documentation site.