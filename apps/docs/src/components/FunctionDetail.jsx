import { useState, useEffect } from 'preact/hooks'
import { Parser } from 'hot-formula-parser'

export function FunctionDetail({ func, category, categoryName }) {
  const [testFormula, setTestFormula] = useState(func.examples[0]?.formula || '')
  const [testResult, setTestResult] = useState('')
  const [xlFormula, setXlFormula] = useState(null)
  const [benchmarkResults, setBenchmarkResults] = useState(null)
  const [isBenchmarking, setIsBenchmarking] = useState(false)

  useEffect(() => {
    initializeXLFormula()
  }, [])

  useEffect(() => {
    if (xlFormula && testFormula) {
      runTest()
    }
  }, [testFormula, xlFormula])

  const initializeXLFormula = async () => {
    try {
      // Import from the workspace package using proper npm dependency
      const XLFormulaModule = await import('xl-formula-web')
      console.log('XL Formula module loaded:', XLFormulaModule)
      
      await XLFormulaModule.default.init()
      console.log('XL Formula initialized successfully')
      
      // Test that it's actually working
      const testResult = XLFormulaModule.default.evaluateNumber('SUM(1,2,3)')
      console.log('XL Formula test result:', testResult)
      
      setXlFormula(XLFormulaModule.default)
    } catch (error) {
      console.error('Failed to initialize XL Formula:', error)
      throw error // Don't fall back to mock, we want to see real errors
    }
  }

  const runTest = () => {
    if (!xlFormula || !testFormula) return

    try {
      let result
      const cleanFormula = testFormula.startsWith('=') ? testFormula.substring(1) : testFormula

      // Determine the expected result type
      if (func.examples.some(ex => ex.formula === testFormula && !isNaN(parseFloat(ex.result)))) {
        result = xlFormula.evaluateNumber(cleanFormula)
      } else if (func.examples.some(ex => ex.formula === testFormula && (ex.result === 'TRUE' || ex.result === 'FALSE'))) {
        result = xlFormula.evaluateBoolean(cleanFormula) ? 'TRUE' : 'FALSE'
      } else {
        result = xlFormula.evaluateText(cleanFormula)
      }

      setTestResult(String(result))
    } catch (error) {
      setTestResult(`Error: ${error.message}`)
    }
  }

  const runBenchmark = async () => {
    if (!xlFormula || !testFormula) return

    setIsBenchmarking(true)
    setBenchmarkResults(null)

    try {
      const iterations = 100000
      const cleanFormula = testFormula.startsWith('=') ? testFormula.substring(1) : testFormula

      console.log('Running benchmark with XL Formula implementation')
      console.log('Formula:', cleanFormula)
      
      // Warm up both implementations
      xlFormula.evaluateNumber(cleanFormula)
      
      // Benchmark XL Formula
      const xlStart = performance.now()
      let xlResult
      for (let i = 0; i < iterations; i++) {
        xlResult = xlFormula.evaluateNumber(cleanFormula)
      }
      const xlTime = performance.now() - xlStart
      console.log('XL Formula result:', xlResult, 'Time:', xlTime, 'ms')

      // Benchmark hot-formula-parser
      let hotFormulaTime = null
      let hotFormulaResult = null
      try {
        // Create Parser instance
        const parser = new Parser()
        
        // Get the result for comparison
        const parseResult = parser.parse(cleanFormula)
        hotFormulaResult = parseResult.error ? null : parseResult.result
        console.log('Hot-formula-parser args:', cleanFormula)
        console.log('Hot-formula-parser parse result:', parseResult)
        
        // Warm up hot-formula-parser
        parser.parse(cleanFormula)
        
        const hfpStart = performance.now()
        for (let i = 0; i < iterations; i++) {
          parser.parse(cleanFormula)
        }
        hotFormulaTime = performance.now() - hfpStart
        console.log('Hot-formula-parser result:', hotFormulaResult, 'Time:', hotFormulaTime, 'ms')
      } catch (error) {
        console.warn('Hot-formula-parser benchmark failed:', error)
      }

      // Verify results match (handle different data types)
      let resultsMatch = false
      if (hotFormulaResult !== null) {
        if (typeof xlResult === 'number' && typeof hotFormulaResult === 'number') {
          resultsMatch = Math.abs(xlResult - hotFormulaResult) < 0.001
        } else {
          resultsMatch = String(xlResult) === String(hotFormulaResult)
        }
      }
      
      if (!resultsMatch && hotFormulaResult !== null) {
        console.warn('Results don\'t match! XL:', xlResult, '(', typeof xlResult, ') Hot-formula-parser:', hotFormulaResult, '(', typeof hotFormulaResult, ')')
      }

      setBenchmarkResults({
        xlFormula: xlTime,
        hotFormulaParser: hotFormulaTime,
        iterations,
        speedup: hotFormulaTime ? (hotFormulaTime / xlTime).toFixed(2) : null,
        xlResult,
        hotFormulaResult,
        resultsMatch: hotFormulaResult !== null ? resultsMatch : null
      })
    } catch (error) {
      console.error('Benchmark failed:', error)
      setBenchmarkResults({
        error: error.message,
        xlFormula: null,
        hotFormulaParser: null,
        iterations: 0,
        speedup: null
      })
    } finally {
      setIsBenchmarking(false)
    }
  }

  return (
    <div>
      {/* Function Header */}
      <div style={{ marginBottom: '2rem' }}>
        <div style={{ marginBottom: '0.25rem' }}>
          <div style={{ display: 'inline-flex', alignItems: 'center', gap: '0.4rem', marginBottom: '0' }}>
            <span className="text-xs rounded" style={{ 
              background: 'var(--color-primary-light)', 
              color: 'var(--color-primary)',
              padding: '2px 8px'
            }}>
              {categoryName}
            </span>
          </div>
          <h1 className="text-3xl font-mono font-bold">{func.name}</h1>
        </div>
        <p className="text-lg text-muted">{func.description}</p>
      </div>

      <div className="grid grid-cols-2 gap-8">
        {/* Documentation */}
        <div>
          {/* Syntax & Parameters */}
          <div className="card mb-6">
            <h3 className="font-semibold mb-4">Syntax & Parameters</h3>
            
            {/* Syntax */}
            <div className="mb-4">
              <div className="text-sm font-medium mb-2">Syntax</div>
              <div className="font-mono p-3 rounded" style={{ background: 'var(--color-bg-secondary)' }}>
                {func.syntax}
              </div>
            </div>

            {/* Parameters */}
            <div>
              <div className="text-sm font-medium mb-3">Parameters</div>
              {func.parameters.length > 0 ? (
                <div className="grid gap-3">
                  {func.parameters.map((param, index) => (
                    <div key={index} className="flex gap-3">
                      <div className="font-mono text-sm font-medium" style={{ minWidth: '100px' }}>
                        {param.name}
                        {param.required && <span className="text-error">*</span>}
                      </div>
                      <div className="text-sm text-muted">{param.description}</div>
                    </div>
                  ))}
                  <div className="text-xs text-muted mt-2">
                    * Required parameter
                  </div>
                </div>
              ) : (
                <p className="text-muted text-sm">This function takes no parameters.</p>
              )}
            </div>
          </div>

          {/* Examples */}
          <div className="card">
            <h3 className="font-semibold mb-3">Examples</h3>
            <div className="grid gap-4">
              {func.examples.map((example, index) => (
                <div key={index} className="p-3 rounded" style={{ background: 'var(--color-bg-secondary)' }}>
                  <div className="flex justify-between items-start mb-2">
                    <code className="font-mono text-sm">{example.formula}</code>
                    <span className="text-sm text-success">{example.result}</span>
                  </div>
                  <div className="text-xs text-muted">{example.description}</div>
                </div>
              ))}
            </div>
          </div>
        </div>

        {/* Interactive Sandbox */}
        <div>
          {/* Try It Out */}
          <div className="card mb-6">
            <h3 className="font-semibold mb-3">Try It Out</h3>
            
            <label className="label">Formula</label>
            <input
              type="text"
              value={testFormula}
              onChange={(e) => setTestFormula(e.target.value)}
              className="input monospace mb-3"
              placeholder="Enter a formula to test"
            />

            <div className="p-3 rounded" style={{ 
              background: 'var(--color-success-light)', 
              border: '1px solid var(--color-success)' 
            }}>
              <div className="flex justify-between items-center">
                <strong>Result:</strong>
                <button onClick={runTest} className="btn btn-sm">
                  Run Test
                </button>
              </div>
              <div className="font-mono mt-2">
                {testResult || 'Click "Run Test" to see result'}
              </div>
            </div>

            <div className="mt-4">
              <h4 className="font-medium mb-2">Quick Examples</h4>
              <div className="grid gap-2">
                {func.examples.map((example, index) => (
                  <button
                    key={index}
                    onClick={() => setTestFormula(example.formula)}
                    className="btn btn-sm text-left"
                    style={{ justifyContent: 'flex-start' }}
                  >
                    <code className="font-mono text-xs">{example.formula}</code>
                  </button>
                ))}
              </div>
            </div>
          </div>

          {/* Benchmark */}
          <div className="card">
            <h3 className="font-semibold mb-3">Performance Benchmark</h3>
            <p className="text-sm text-muted mb-4">
              Compare XL Formula performance against Hot Formula Parser
            </p>

            <button
              onClick={runBenchmark}
              disabled={isBenchmarking || !testFormula}
              className="btn btn-primary mb-4"
            >
              {isBenchmarking ? 'Running Benchmark...' : 'Run Benchmark'}
            </button>

            {benchmarkResults && (
              <div className="grid gap-3">
                {benchmarkResults.error ? (
                  <div className="p-3 rounded" style={{ background: 'var(--color-error-light)', color: 'var(--color-error)' }}>
                    <strong>Benchmark Error:</strong> {benchmarkResults.error}
                  </div>
                ) : (
                  <>

                    
                    <div className="flex justify-between">
                      <span>XL Formula:</span>
                      <span className="font-mono">
                        {benchmarkResults.xlFormula ? `${benchmarkResults.xlFormula.toFixed(2)}ms` : 'N/A'}
                      </span>
                    </div>
                    
                    {benchmarkResults.hotFormulaParser && (
                      <>
                        <div className="flex justify-between">
                          <span>Hot Formula Parser:</span>
                          <span className="font-mono">{benchmarkResults.hotFormulaParser.toFixed(2)}ms</span>
                        </div>
                        <div className="flex justify-between font-semibold">
                          <span>Speedup:</span>
                          <span className={benchmarkResults.speedup > 1 ? "text-success" : "text-error"}>
                            {benchmarkResults.speedup > 1 ? `${benchmarkResults.speedup}x faster` : `${(1/parseFloat(benchmarkResults.speedup)).toFixed(2)}x slower`}
                          </span>
                        </div>
                      </>
                    )}
                    
                    {benchmarkResults.resultsMatch !== null && (
                      <div className="flex justify-between text-xs">
                        <span>Results match:</span>
                        <span className={benchmarkResults.resultsMatch ? "text-success" : "text-error"}>
                          {benchmarkResults.resultsMatch ? '✓ Yes' : '✗ No'} 
                          {!benchmarkResults.resultsMatch && ` (XL: ${benchmarkResults.xlResult}, HFP: ${benchmarkResults.hotFormulaResult})`}
                        </span>
                      </div>
                    )}
                    
                    <div className="text-xs text-muted">
                      Tested with {benchmarkResults.iterations.toLocaleString()} iterations
                    </div>
                  </>
                )}
              </div>
            )}

            {!benchmarkResults && !isBenchmarking && (
              <div className="text-sm text-muted">
                Enter a formula above and click "Run Benchmark" to compare performance against Hot Formula Parser.
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  )
}