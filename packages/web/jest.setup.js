// Jest setup for XL Formula WASM testing
import { TextEncoder, TextDecoder } from 'util';
import XMLHttpRequest from 'xhr2';
import fs from 'fs';
import path from 'path';

// Make Node.js globals available in jsdom environment
global.TextEncoder = TextEncoder;
global.TextDecoder = TextDecoder;

// Set up environment for WASM loading
global.XMLHttpRequest = XMLHttpRequest;

// Mock performance.now if not available
if (typeof performance === 'undefined') {
  global.performance = {
    now: () => Date.now()
  };
}

// Mock fetch to load actual WASM file from filesystem
global.fetch = global.fetch || ((url) => {
  // If it's requesting the WASM file, load it from filesystem
  if (url.includes('xl-formula.wasm') || url.endsWith('.wasm')) {
    const wasmPath = path.resolve(process.cwd(), 'xl-formula.wasm');
    
    try {
      if (fs.existsSync(wasmPath)) {
        const wasmBuffer = fs.readFileSync(wasmPath);
        
        return Promise.resolve({
          ok: true,
          arrayBuffer: () => Promise.resolve(wasmBuffer.buffer.slice(wasmBuffer.byteOffset, wasmBuffer.byteOffset + wasmBuffer.byteLength))
        });
      } else {
        console.error('❌ WASM file not found at:', wasmPath);
        return Promise.reject(new Error(`WASM file not found: ${wasmPath}`));
      }
    } catch (error) {
      console.error('❌ Error loading WASM file:', error);
      return Promise.reject(error);
    }
  }
  
  // For other requests, return empty response
  return Promise.resolve({
    ok: true,
    arrayBuffer: () => Promise.resolve(new ArrayBuffer(0))
  });
});

// Global WASM initialization for all tests
let globalXLFormulaModule = null;
let globalFormulaEngine = null;

// Check if WASM files exist
const wasmPath = path.resolve(process.cwd(), 'xl-formula.js');
const wrapperPath = path.resolve(process.cwd(), 'xl-formula-wrapper.js');
const hasWasmFiles = fs.existsSync(wasmPath) && fs.existsSync(wrapperPath);

// Initialize WASM globally
const initializeWasm = async () => {
  if (!hasWasmFiles) {
    console.log('⚠️  Skipping WASM initialization - files not found');
    console.log(`Looking for: ${wasmPath}, ${wrapperPath}`);
    return false;
  }

  try {
    console.log('🚀 Initializing XL Formula WASM globally...');
    
    // Temporarily suppress console.error to avoid Emscripten noise
    const originalConsoleError = console.error;
    console.error = (...args) => {
      // Suppress Emscripten streaming compilation noise
      const message = args.join(' ');
      if (!message.includes('falling back to ArrayBuffer instantiation') && 
          !message.includes('wasm streaming compile failed')) {
        originalConsoleError(...args);
      }
    };
    
    // Load the wrapper module using dynamic import
    const WrapperModule = await import('./xl-formula-wrapper.js');
    globalXLFormulaModule = WrapperModule.default;
    
    console.log('📦 Wrapper module loaded, initializing WASM...');
    
    // Initialize the WASM module with proper async handling
    const initResult = await globalXLFormulaModule.init();
    console.log('🔄 Global init result:', initResult);
    
    // Give it a moment to fully initialize
    await new Promise(resolve => setTimeout(resolve, 100));
    
    globalFormulaEngine = new globalXLFormulaModule.FormulaEngine();
    console.log('✅ XL Formula initialized globally - ready for all tests');
    
    // Restore console.error
    console.error = originalConsoleError;
    
    return true;
  } catch (error) {
    console.error('❌ Failed to initialize XL Formula globally:', error);
    console.error('Error details:', error.message);
    return false;
  }
};

// Make globals available to all tests
global.getXLFormulaModule = () => globalXLFormulaModule;
global.getFormulaEngine = () => globalFormulaEngine;
global.hasWasmAvailable = () => hasWasmFiles && globalXLFormulaModule && globalFormulaEngine;

// Initialize WASM before any tests run
global.wasmInitializationPromise = initializeWasm();