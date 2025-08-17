import { Link } from 'preact-router/match'
import { useState, useRef, useEffect } from 'preact/hooks'

export function Header() {
  // Get base path from environment variable, default to '/' for development
  const basePath = import.meta.env.VITE_BASE_PATH || '/'
  const [mobileMenuOpen, setMobileMenuOpen] = useState(false)
  const [formulasDropdownOpen, setFormulasDropdownOpen] = useState(false)
  const dropdownTimeoutRef = useRef(null)
  const dropdownRef = useRef(null)

  // Clear timeout when component unmounts
  useEffect(() => {
    return () => {
      if (dropdownTimeoutRef.current) {
        clearTimeout(dropdownTimeoutRef.current)
      }
    }
  }, [])

  const handleDropdownMouseEnter = () => {
    if (dropdownTimeoutRef.current) {
      clearTimeout(dropdownTimeoutRef.current)
    }
    setFormulasDropdownOpen(true)
  }

  const handleDropdownMouseLeave = () => {
    dropdownTimeoutRef.current = setTimeout(() => {
      setFormulasDropdownOpen(false)
    }, 150) // Small delay to allow moving to dropdown
  }

  return (
    <header style={{ 
      borderBottom: '1px solid var(--color-border)', 
      background: 'var(--color-bg)',
      position: 'sticky',
      top: 0,
      zIndex: 50
    }}>
      <div className="container">
        <nav className="flex items-center justify-between" style={{ height: '4rem' }}>
          <div className="flex items-center" style={{ flex: 1 }}>
            <Link href={basePath} className="text-xl font-bold" style={{ textDecoration: 'none', color: 'var(--color-primary)' }}>
              Velox
            </Link>
            <button className="show-mobile" style={{ marginLeft: 'auto', background: 'none', border: 'none', color: 'var(--color-text)', fontSize: '1.5rem', display: 'none', padding: '0.25rem 0.5rem' }} aria-label="Open menu" onClick={() => setMobileMenuOpen(true)}>
              <span style={{ fontSize: '1.75rem', lineHeight: 1 }}>&#9776;</span>
            </button>
            <div className="flex items-center hide-mobile" style={{ marginLeft: '1rem' }}>
              {/* Formulas Dropdown */}
              <div 
                style={{ position: 'relative' }}
                onMouseEnter={handleDropdownMouseEnter}
                onMouseLeave={handleDropdownMouseLeave}
                ref={dropdownRef}
              >
                <button 
                  className="text-sm font-medium nav-link"
                  style={{ 
                    textDecoration: 'none', 
                    color: 'var(--color-text-secondary)',
                    padding: '6px 10px',
                    borderRadius: '6px',
                    transition: 'all 0.2s ease',
                    background: 'none',
                    border: 'none',
                    cursor: 'pointer',
                    display: 'flex',
                    alignItems: 'center',
                    gap: '4px'
                  }}
                >
                  Formulas
                  <span style={{ fontSize: '0.75rem' }}>▼</span>
                </button>
                {formulasDropdownOpen && (
                  <div 
                    style={{
                      position: 'absolute',
                      top: '100%',
                      left: 0,
                      background: 'var(--color-bg)',
                      border: '1px solid var(--color-border)',
                      borderRadius: '8px',
                      boxShadow: '0 4px 6px -1px rgba(0, 0, 0, 0.1)',
                      minWidth: '160px',
                      zIndex: 100,
                      marginTop: '4px' // Add small gap to prevent immediate close
                    }}
                  >
                    <Link 
                      href={`${basePath}formulas`}
                      style={{
                        display: 'block',
                        padding: '8px 12px',
                        textDecoration: 'none',
                        color: 'var(--color-text)',
                        fontSize: '0.875rem',
                        borderBottom: '1px solid var(--color-border)'
                      }}
                      onMouseOver={(e) => e.target.style.backgroundColor = 'var(--color-bg-secondary)'}
                      onMouseOut={(e) => e.target.style.backgroundColor = 'transparent'}
                    >
                      Overview
                    </Link>
                    <Link 
                      href={`${basePath}formulas/playground`}
                      style={{
                        display: 'block',
                        padding: '8px 12px',
                        textDecoration: 'none',
                        color: 'var(--color-text)',
                        fontSize: '0.875rem',
                        borderBottom: '1px solid var(--color-border)'
                      }}
                      onMouseOver={(e) => e.target.style.backgroundColor = 'var(--color-bg-secondary)'}
                      onMouseOut={(e) => e.target.style.backgroundColor = 'transparent'}
                    >
                      Playground
                    </Link>
                    <Link 
                      href={`${basePath}formulas/docs`}
                      style={{
                        display: 'block',
                        padding: '8px 12px',
                        textDecoration: 'none',
                        color: 'var(--color-text)',
                        fontSize: '0.875rem'
                      }}
                      onMouseOver={(e) => e.target.style.backgroundColor = 'var(--color-bg-secondary)'}
                      onMouseOut={(e) => e.target.style.backgroundColor = 'transparent'}
                    >
                      Documentation
                    </Link>
                  </div>
                )}
              </div>
            </div>
          </div>
          <div className="flex items-center gap-4 hide-mobile">
            <a 
              href="https://github.com/rpenfold/xl-formula" 
              target="_blank" 
              rel="noopener noreferrer"
              className="btn btn-sm"
            >
              GitHub
            </a>
            <a 
              href="https://www.npmjs.com/package/@velox/formulas" 
              target="_blank" 
              rel="noopener noreferrer"
              className="btn btn-sm btn-primary"
            >
              npm install
            </a>
          </div>
        </nav>
      </div>
      {/* Mobile Drawer */}
      {mobileMenuOpen && (
        <>
          <div className="sidebar-mobile-backdrop" onClick={() => setMobileMenuOpen(false)}></div>
          <div className="sidebar-mobile-drawer open">
            <div style={{ padding: '1rem', borderBottom: '1px solid var(--color-border)', display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
              <span className="text-xl font-bold" style={{ color: 'var(--color-primary)' }}>Velox</span>
              <button className="btn btn-sm" style={{ background: 'none', border: 'none', fontSize: '1.5rem' }} aria-label="Close menu" onClick={() => setMobileMenuOpen(false)}>&times;</button>
            </div>
            <nav style={{ display: 'flex', flexDirection: 'column', padding: '1rem' }}>
              <Link href={basePath} className="btn btn-sm" style={{ marginBottom: '0.5rem' }} onClick={() => setMobileMenuOpen(false)}>
                Home
              </Link>
              <div style={{ marginBottom: '0.5rem' }}>
                <div className="btn btn-sm" style={{ width: '100%', textAlign: 'left', background: 'var(--color-bg-secondary)' }}>
                  Formulas
                </div>
                <div style={{ paddingLeft: '1rem', marginTop: '0.25rem' }}>
                  <Link href={`${basePath}formulas`} className="btn btn-sm" style={{ display: 'block', marginBottom: '0.25rem', fontSize: '0.875rem' }} onClick={() => setMobileMenuOpen(false)}>
                    Overview
                  </Link>
                  <Link href={`${basePath}formulas/playground`} className="btn btn-sm" style={{ display: 'block', marginBottom: '0.25rem', fontSize: '0.875rem' }} onClick={() => setMobileMenuOpen(false)}>
                    Playground
                  </Link>
                  <Link href={`${basePath}formulas/docs`} className="btn btn-sm" style={{ display: 'block', fontSize: '0.875rem' }} onClick={() => setMobileMenuOpen(false)}>
                    Documentation
                  </Link>
                </div>
              </div>
              <a href="https://github.com/rpenfold/xl-formula" target="_blank" rel="noopener noreferrer" className="btn btn-sm">
                GitHub
              </a>
              <a href="https://www.npmjs.com/package/@velox/formulas" target="_blank" rel="noopener noreferrer" className="btn btn-sm btn-primary">
                npm install
              </a>
            </nav>
          </div>
        </>
      )}
    </header>
  )
}