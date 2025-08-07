// XL Formula loader utility for Vite
let xlFormulaPromise = null

export async function loadXLFormula() {
  if (xlFormulaPromise) {
    return xlFormulaPromise
  }

  xlFormulaPromise = new Promise((resolve, reject) => {
    // Create and inject the script tag
    const script = document.createElement('script')
    script.type = 'module'
    script.textContent = `
      import('/xl-formula-wrapper.js?url').then(async (module) => {
        const wrapperUrl = module.default
        const response = await fetch(wrapperUrl)
        const wrapperCode = await response.text()
        
        // Create a data URL for the wrapper code
        const blob = new Blob([wrapperCode], { type: 'application/javascript' })
        const moduleUrl = URL.createObjectURL(blob)
        
        // Import the wrapper module
        const xlFormulaModule = await import(moduleUrl)
        
        // Initialize the module
        await xlFormulaModule.default.init()
        
        // Cleanup
        URL.revokeObjectURL(moduleUrl)
        
        // Resolve with the initialized module
        window.XLFormulaModule = xlFormulaModule.default
        window.postMessage({ type: 'xl-formula-loaded', module: xlFormulaModule.default }, '*')
      }).catch(error => {
        window.postMessage({ type: 'xl-formula-error', error: error.message }, '*')
      })
    `

    // Listen for the result
    const messageHandler = (event) => {
      if (event.data?.type === 'xl-formula-loaded') {
        window.removeEventListener('message', messageHandler)
        document.head.removeChild(script)
        resolve(event.data.module)
      } else if (event.data?.type === 'xl-formula-error') {
        window.removeEventListener('message', messageHandler)
        document.head.removeChild(script)
        reject(new Error(event.data.error))
      }
    }

    window.addEventListener('message', messageHandler)
    
    // Add error handling for script loading
    script.onerror = () => {
      window.removeEventListener('message', messageHandler)
      document.head.removeChild(script)
      reject(new Error('Failed to load XL Formula script'))
    }

    document.head.appendChild(script)
  })

  return xlFormulaPromise
}