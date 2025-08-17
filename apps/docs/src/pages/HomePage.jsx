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
        padding: '6rem 0',
      }}>
        <div className="container" style={{ textAlign: 'center' }}>
          <h1 className="text-3xl font-bold mb-4" style={{ fontSize: '3rem', marginBottom: '1.5rem' }}>
            Velox
          </h1>
          <p className="text-xl mb-8" style={{ fontSize: '1.25rem', marginBottom: '2rem', opacity: 0.9 }}>
            High-performance libraries for modern web applications
          </p>
          <div className="flex gap-4" style={{ justifyContent: 'center', flexWrap: 'wrap' }}>
            <Link href={`${basePath}formulas`} className="btn btn-primary" style={{ 
              background: 'white', 
              color: '#4f46e5',
              padding: '0.875rem 1.25rem',
              fontSize: '0.95rem',
              textDecoration: 'none',
              marginBottom: '0.5rem',
              flex: '0 1 200px',
              minWidth: '140px',
              maxWidth: '220px',
            }}>
              Explore Formulas
            </Link>
            <a href="https://github.com/rpenfold/xl-formula" target="_blank" rel="noopener noreferrer" className="btn" style={{ 
              background: 'rgba(255,255,255,0.1)', 
              border: '1px solid rgba(255,255,255,0.3)',
              color: 'white',
              padding: '0.875rem 1.25rem',
              fontSize: '0.95rem',
              textDecoration: 'none',
              marginBottom: '0.5rem',
              flex: '0 1 200px',
              minWidth: '140px',
              maxWidth: '220px',
            }}>
              View on GitHub
            </a>
          </div>
        </div>
      </section>

      {/* About Section */}
      <section style={{ padding: '4rem 0' }}>
        <div className="container">
          <div className="grid grid-cols-2 gap-12 items-center">
            <div>
              <h2 className="text-3xl font-bold mb-6">
                Building the Future of Web Performance
              </h2>
              <p className="text-lg mb-6" style={{ color: 'var(--color-text-secondary)' }}>
                Velox is a collection of high-performance libraries designed to bring native-level performance to web applications. 
                Our libraries are built with WebAssembly and modern web technologies to deliver exceptional speed and reliability.
              </p>
              <p className="text-lg mb-8" style={{ color: 'var(--color-text-secondary)' }}>
                Whether you're building data-intensive applications, real-time dashboards, or complex computational tools, 
                Velox provides the performance foundation you need.
              </p>
            </div>
            <div style={{ textAlign: 'center' }}>
              <div style={{ 
                width: '12rem', 
                height: '12rem', 
                background: 'var(--color-primary-light)', 
                borderRadius: '50%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                margin: '0 auto',
                fontSize: '4rem'
              }}>
                ⚡
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* Packages Section */}
      <section style={{ padding: '4rem 0', background: 'var(--color-bg-secondary)' }}>
        <div className="container">
          <h2 className="text-2xl font-bold mb-8" style={{ textAlign: 'center' }}>
            Our Libraries
          </h2>
          <div className="grid grid-cols-1 gap-8" style={{ maxWidth: '800px', margin: '0 auto' }}>
            <div className="card" style={{ 
              padding: '2rem',
              border: '1px solid var(--color-border)',
              borderRadius: '12px',
              background: 'var(--color-bg)',
              transition: 'transform 0.2s ease, box-shadow 0.2s ease'
            }}>
              <div className="flex items-center justify-between">
                <div>
                  <h3 className="text-xl font-bold mb-2">Velox Formulas</h3>
                  <p className="text-muted mb-4">
                    High-performance Excel-like formula parser and evaluator. Supports 100+ Excel functions 
                    with WebAssembly-powered performance for complex calculations in the browser.
                  </p>
                  <div className="flex gap-2">
                    <span className="badge" style={{ 
                      background: 'var(--color-primary-light)', 
                      color: 'var(--color-primary)',
                      padding: '0.25rem 0.5rem',
                      borderRadius: '4px',
                      fontSize: '0.75rem'
                    }}>
                      WebAssembly
                    </span>
                    <span className="badge" style={{ 
                      background: 'var(--color-primary-light)', 
                      color: 'var(--color-primary)',
                      padding: '0.25rem 0.5rem',
                      borderRadius: '4px',
                      fontSize: '0.75rem'
                    }}>
                      Excel Compatible
                    </span>
                    <span className="badge" style={{ 
                      background: 'var(--color-primary-light)', 
                      color: 'var(--color-primary)',
                      padding: '0.25rem 0.5rem',
                      borderRadius: '4px',
                      fontSize: '0.75rem'
                    }}>
                      TypeScript
                    </span>
                  </div>
                </div>
                <Link href={`${basePath}formulas`} className="btn btn-primary" style={{ 
                  background: 'var(--color-primary)',
                  color: 'white',
                  padding: '0.5rem 1rem',
                  textDecoration: 'none',
                  borderRadius: '6px',
                  fontSize: '0.875rem'
                }}>
                  Learn More
                </Link>
              </div>
            </div>
            
            <div className="card" style={{ 
              padding: '2rem',
              border: '1px solid var(--color-border)',
              borderRadius: '12px',
              background: 'var(--color-bg)',
              opacity: 0.6
            }}>
              <div className="flex items-center justify-between">
                <div>
                  <h3 className="text-xl font-bold mb-2">More Libraries Coming Soon</h3>
                  <p className="text-muted mb-4">
                    We're working on additional high-performance libraries for data processing, 
                    visualization, and more. Stay tuned for updates!
                  </p>
                  <div className="flex gap-2">
                    <span className="badge" style={{ 
                      background: 'var(--color-bg-secondary)', 
                      color: 'var(--color-text-secondary)',
                      padding: '0.25rem 0.5rem',
                      borderRadius: '4px',
                      fontSize: '0.75rem'
                    }}>
                      Coming Soon
                    </span>
                  </div>
                </div>
                <button className="btn" style={{ 
                  background: 'var(--color-bg-secondary)',
                  color: 'var(--color-text-secondary)',
                  padding: '0.5rem 1rem',
                  border: '1px solid var(--color-border)',
                  borderRadius: '6px',
                  fontSize: '0.875rem',
                  cursor: 'not-allowed'
                }} disabled>
                  Coming Soon
                </button>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* Features Section */}
      <section style={{ padding: '4rem 0' }}>
        <div className="container">
          <h2 className="text-2xl font-bold mb-8" style={{ textAlign: 'center' }}>
            Why Choose Velox?
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
                🚀
              </div>
              <h3 className="font-semibold mb-2">Native Performance</h3>
              <p className="text-muted">
                WebAssembly compilation delivers near-native performance in the browser
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
                🛠️
              </div>
              <h3 className="font-semibold mb-2">Developer Experience</h3>
              <p className="text-muted">
                TypeScript support, comprehensive documentation, and easy integration
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
                🔒
              </div>
              <h3 className="font-semibold mb-2">Production Ready</h3>
              <p className="text-muted">
                Battle-tested libraries with comprehensive test coverage and active maintenance
              </p>
            </div>
          </div>
        </div>
      </section>
    </div>
  )
}