import { Link } from 'preact-router/match'
import { useState } from 'preact/hooks'

export function Header() {
  // Get base path from environment variable, default to '/' for development
  const basePath = import.meta.env.VITE_BASE_PATH || '/'
  const [mobileMenuOpen, setMobileMenuOpen] = useState(false)

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
              XL Formula
            </Link>
            <button className="show-mobile" style={{ marginLeft: 'auto', background: 'none', border: 'none', color: 'var(--color-text)', fontSize: '1.5rem', display: 'none', padding: '0.25rem 0.5rem' }} aria-label="Open menu" onClick={() => setMobileMenuOpen(true)}>
              <span style={{ fontSize: '1.75rem', lineHeight: 1 }}>&#9776;</span>
            </button>
            <div className="flex items-center hide-mobile" style={{ marginLeft: '1rem' }}>
              <Link 
                href={`${basePath}playground`} 
                className="text-sm font-medium nav-link"
                style={{ 
                  textDecoration: 'none', 
                  color: 'var(--color-text-secondary)',
                  padding: '6px 10px',
                  borderRadius: '6px',
                  transition: 'all 0.2s ease'
                }}
                activeClassName="text-primary"
                onMouseOver={(e) => e.target.style.backgroundColor = 'var(--color-bg-secondary)'}
                onMouseOut={(e) => e.target.style.backgroundColor = 'transparent'}
              >
                Playground
              </Link>
              <Link 
                href={`${basePath}docs`} 
                className="text-sm font-medium nav-link"
                style={{ 
                  textDecoration: 'none', 
                  color: 'var(--color-text-secondary)',
                  padding: '6px 10px',
                  borderRadius: '6px',
                  transition: 'all 0.2s ease'
                }}
                activeClassName="text-primary"
                onMouseOver={(e) => e.target.style.backgroundColor = 'var(--color-bg-secondary)'}
                onMouseOut={(e) => e.target.style.backgroundColor = 'transparent'}
              >
                Documentation
              </Link>
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
              href="https://www.npmjs.com/package/xl-formula-web" 
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
              <span className="text-xl font-bold" style={{ color: 'var(--color-primary)' }}>XL Formula</span>
              <button className="btn btn-sm" style={{ background: 'none', border: 'none', fontSize: '1.5rem' }} aria-label="Close menu" onClick={() => setMobileMenuOpen(false)}>&times;</button>
            </div>
            <nav style={{ display: 'flex', flexDirection: 'column', padding: '1rem' }}>
              <Link href={basePath} className="btn btn-sm" style={{ marginBottom: '0.5rem' }} onClick={() => setMobileMenuOpen(false)}>
                Home
              </Link>
              <Link href={`${basePath}playground`} className="btn btn-sm" onClick={() => setMobileMenuOpen(false)}>
                Playground
              </Link>
              <Link href={`${basePath}docs`} className="btn btn-sm" onClick={() => setMobileMenuOpen(false)}>
                Documentation
              </Link>
              <a href="https://github.com/rpenfold/xl-formula" target="_blank" rel="noopener noreferrer" className="btn btn-sm">
                GitHub
              </a>
              <a href="https://www.npmjs.com/package/xl-formula-web" target="_blank" rel="noopener noreferrer" className="btn btn-sm btn-primary">
                npm install
              </a>
            </nav>
          </div>
        </>
      )}
    </header>
  )
}