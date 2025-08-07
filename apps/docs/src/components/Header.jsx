import { Link } from 'preact-router/match'

export function Header() {
  // Get base path from environment variable, default to '/' for development
  const basePath = import.meta.env.VITE_BASE_PATH || '/'
  
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
          <div className="flex items-center">
            <Link href={basePath} className="text-xl font-bold" style={{ textDecoration: 'none', color: 'var(--color-primary)' }}>
              XL Formula
            </Link>
            
            <div className="flex items-center">
              <Link 
                href={`${basePath}playground`} 
                className="text-sm font-medium nav-link"
                style={{ 
                  textDecoration: 'none', 
                  color: 'var(--color-text-secondary)',
                  padding: '8px 16px',
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
                  padding: '8px 16px',
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
          
          <div className="flex items-center gap-4">
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
    </header>
  )
}