import { defineConfig } from 'vite'
import preact from '@preact/preset-vite'
import { resolve } from 'path'

export default defineConfig({
  plugins: [preact()],
  base: process.env.NODE_ENV === 'production' ? '/xl-formula/' : '/',
  resolve: {
    alias: {
      'xl-formula-web': resolve(__dirname, '../../node_modules/xl-formula-web/xl-formula-wrapper.js')
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