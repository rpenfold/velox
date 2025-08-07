import { defineConfig } from 'vite'
import preact from '@preact/preset-vite'
import { resolve } from 'path'

// Get base path from environment variable, default to '/' for development
const basePath = process.env.VITE_BASE_PATH || '/'

export default defineConfig({
  plugins: [preact()],
  base: basePath,
  resolve: {
    alias: {
      'xl-formula-web': resolve(__dirname, '../../packages/web/xl-formula-wrapper.js')
    }
  },
  build: {
    outDir: 'dist',
    rollupOptions: {
      output: {
        manualChunks: {
          vendor: ['preact', 'preact-router'],
          formula: ['hot-formula-parser']
        }
      }
    }
  },
  server: {
    port: 3000,
    open: true,
    fs: {
      allow: ['..', '../..'] // Allow serving files from workspace root
    }
  },
  assetsInclude: ['**/*.wasm'], // Treat WASM files as assets
  optimizeDeps: {
    exclude: ['xl-formula-web'] // Don't pre-bundle our WASM module
  }
})