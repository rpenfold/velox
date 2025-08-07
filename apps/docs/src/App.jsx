import { Router } from 'preact-router'
import { Header } from './components/Header.jsx'
import { HomePage } from './pages/HomePage.jsx'
import { DocsPage } from './pages/DocsPage.jsx'
import { PlaygroundPage } from './pages/PlaygroundPage.jsx'

export function App() {
  // Get base path from environment variable, default to '/' for development
  const basePath = import.meta.env.VITE_BASE_PATH || '/'
  
  return (
    <div style={{ minHeight: '100vh', display: 'flex', flexDirection: 'column' }}>
      <Header />
      <main style={{ flex: 1 }}>
        <Router>
          <HomePage path={basePath} />
          <HomePage path={`${basePath}`} />
          <DocsPage path={`${basePath}docs`} />
          <DocsPage path={`${basePath}docs/:category?`} />
          <DocsPage path={`${basePath}docs/:category/:function?`} />
          <PlaygroundPage path={`${basePath}playground`} />
        </Router>
      </main>
    </div>
  )
}