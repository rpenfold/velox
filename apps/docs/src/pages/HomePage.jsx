import { Link } from 'preact-router'

// Get base path from environment variable, default to '/' for development
const basePath = import.meta.env.VITE_BASE_PATH || '/'

export function HomePage() {
  return (
    <div>
      {/* Hero Section */}
      <section style={{ 
        background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
        color: 'white',
        padding: '6rem 0'
      }}>
        <div className="container" style={{ textAlign: 'center' }}>
          <h1 className="text-3xl font-bold mb-4" style={{ fontSize: '3rem', marginBottom: '1.5rem' }}>
            XL Formula
          </h1>
          <p className="text-xl mb-8" style={{ fontSize: '1.25rem', marginBottom: '2rem', opacity: 0.9 }}>
            High-performance Excel-like formula parser and evaluator for web applications
          </p>
          <div className="flex gap-4" style={{ justifyContent: 'center' }}>
            <Link href={`${basePath}playground`} className="btn btn-primary" style={{ 
              background: 'white', 
              color: '#4f46e5',
              padding: '1rem 2rem',
              fontSize: '1rem',
              textDecoration: 'none'
            }}>
              Try Playground
            </Link>
            <Link href={`${basePath}docs`} className="btn" style={{ 
              background: 'rgba(255,255,255,0.1)', 
              border: '1px solid rgba(255,255,255,0.3)',
              color: 'white',
              padding: '1rem 2rem',
              fontSize: '1rem',
              textDecoration: 'none'
            }}>
              Documentation
            </Link>
          </div>
        </div>
      </section>

      {/* Features Section */}
      <section style={{ padding: '4rem 0' }}>
        <div className="container">
          <h2 className="text-2xl font-bold mb-8" style={{ textAlign: 'center' }}>
            Why XL Formula?
          </h2>
          
          <div className="grid grid-cols-3 gap-8">
            <div className="card" style={{ textAlign: 'center' }}>
              <div style={{ 
                width: '3rem', 
                height: '3rem', 
                background: 'var(--color-primary-light)', 
                borderRadius: '50%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                margin: '0 auto 1rem'
              }}>
                ⚡
              </div>
              <h3 className="font-semibold mb-2">Lightning Fast</h3>
              <p className="text-muted">
                Compiled to WebAssembly for near-native performance in the browser
              </p>
            </div>
            
            <div className="card" style={{ textAlign: 'center' }}>
              <div style={{ 
                width: '3rem', 
                height: '3rem', 
                background: 'var(--color-primary-light)', 
                borderRadius: '50%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                margin: '0 auto 1rem'
              }}>
                📊
              </div>
              <h3 className="font-semibold mb-2">Excel Compatible</h3>
              <p className="text-muted">
                Supports 100+ Excel functions with familiar syntax and behavior
              </p>
            </div>
            
            <div className="card" style={{ textAlign: 'center' }}>
              <div style={{ 
                width: '3rem', 
                height: '3rem', 
                background: 'var(--color-primary-light)', 
                borderRadius: '50%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                margin: '0 auto 1rem'
              }}>
                🔧
              </div>
              <h3 className="font-semibold mb-2">Easy Integration</h3>
              <p className="text-muted">
                Simple JavaScript API with TypeScript support and zero dependencies
              </p>
            </div>
          </div>
        </div>
      </section>

      {/* Quick Start Section */}
      <section style={{ padding: '4rem 0', background: 'var(--color-bg-secondary)' }}>
        <div className="container">
          <h2 className="text-2xl font-bold mb-8" style={{ textAlign: 'center' }}>
            Quick Start
          </h2>
          
          <div className="grid grid-cols-2 gap-8">
            <div>
              <h3 className="font-semibold mb-4">Installation</h3>
              <pre className="p-4 rounded" style={{ 
                background: 'var(--color-bg)', 
                border: '1px solid var(--color-border)',
                fontFamily: 'var(--font-mono)',
                fontSize: '0.875rem'
              }}>
                <code>npm install xl-formula-web</code>
              </pre>
            </div>
            
            <div>
              <h3 className="font-semibold mb-4">Basic Usage</h3>
              <pre className="p-4 rounded" style={{ 
                background: 'var(--color-bg)', 
                border: '1px solid var(--color-border)',
                fontFamily: 'var(--font-mono)',
                fontSize: '0.875rem'
              }}>
                <code>{`import XLFormula from 'xl-formula-web'

await XLFormula.init()
const result = XLFormula.evaluate('SUM(1,2,3)')
if (result.isSuccess()) {
  console.log(result.getValue().asNumber()) // 6
}`}</code>
              </pre>
            </div>
          </div>
        </div>
      </section>
    </div>
  )
}