# Velox Project Structure

This project has been restructured into a monorepo with clear separation between the distributable package and documentation/demo applications.

## Directory Structure

```
velox/
├── package.json           # Root package.json with workspace configuration
├── packages/web/          # NPM package for distribution
│   ├── xl-formula.js      # Emscripten-generated JavaScript
│   ├── xl-formula.wasm    # WebAssembly binary
│   ├── xl-formula-wrapper.js  # Clean JavaScript API
│   ├── xl-formula.d.ts    # TypeScript definitions
│   ├── package.json       # NPM package configuration
│   └── README.md          # Package documentation
│
├── apps/docs/             # Documentation and demo site
│   ├── src/               # Preact source code
│   │   ├── components/    # React components
│   │   ├── pages/         # Page components
│   │   ├── data/          # Function documentation data
│   │   └── App.jsx        # Main app component
│   ├── node_modules/      # Dependencies (includes symlink to packages/web)
│   ├── index.html         # HTML template
│   ├── package.json       # Docs app configuration
│   └── vite.config.js     # Vite build configuration
│
├── cpp/                   # C++ source code
│   ├── core/             # Core library
│   ├── functions/        # Function implementations
│   ├── bindings/web/     # WebAssembly bindings
│   └── include/          # Header files
│
├── tests/                # Test suites
├── examples/             # Usage examples
├── build.sh              # Enhanced build script
└── CMakeLists.txt        # Main CMake configuration
```

## Applications

### 📦 packages/web (@velox/formulas)
The distributable NPM package containing:
- WebAssembly binaries
- JavaScript wrapper with clean API
- TypeScript definitions
- Documentation

**Usage:**
```bash
cd packages/web
npm publish
```

### 📖 apps/docs
Interactive documentation site built with Preact and Vite featuring:
- Function documentation with examples
- Interactive playground
- Performance benchmarking
- Responsive design
- Static site generation

**Development:**
```bash
cd apps/docs
npm run dev      # Start dev server at http://localhost:3000
npm run build    # Build for production
npm run preview  # Preview production build
```

## Building

This project uses npm workspaces for dependency management. The `packages/web` package is automatically linked to `apps/docs` via workspace dependencies.

### WebAssembly Package
```bash
npm run build
# or directly: ./build.sh --web
```
This builds the WebAssembly binaries and copies them to `packages/web/`.

### Documentation Site
```bash
npm run dev        # Start development server
npm run build:docs # Build for production
npm run preview:docs # Preview production build
```

The docs app automatically imports the web package via the workspace link, eliminating file duplication.

## Features

### Documentation Site Features
- **Interactive Playground**: Test formulas in real-time
- **Function Documentation**: Detailed docs for 100+ functions
- **Performance Benchmarking**: Compare against FormulaJS
- **Responsive Design**: Works on desktop and mobile
- **Static Generation**: Fast loading and SEO-friendly
- **Search**: Find functions by name or category

### Package Features  
- **WebAssembly Performance**: Near-native speed
- **Excel Compatibility**: Familiar syntax and functions
- **TypeScript Support**: Full type definitions
- **Zero Dependencies**: Self-contained package
- **Modern APIs**: ES6 modules with CommonJS fallback

## Deployment

### NPM Package
The `packages/web` directory is ready for NPM publishing:
```bash
cd packages/web
npm publish
```

### Documentation Site
The docs app builds to static files that can be deployed anywhere:
```bash
cd apps/docs
npm run build
# Deploy the dist/ directory to your hosting provider
```

Recommended hosting:
- **Vercel**: Automatic deployment from Git
- **Netlify**: Drag & drop deployment  
- **GitHub Pages**: Free static hosting
- **Cloudflare Pages**: Fast global CDN