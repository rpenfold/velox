import { Router } from 'preact-router'
import { Header } from './components/Header.jsx'
import { HomePage } from './pages/HomePage.jsx'
import { DocsPage } from './pages/DocsPage.jsx'
import { PlaygroundPage } from './pages/PlaygroundPage.jsx'
import { getRuntimeBasePath, getCurrentPath } from './utils/basePath.js'

export function App() {
  const runtimeBasePath = getRuntimeBasePath()
  const currentPath = getCurrentPath()
  
  return (
    <div style={{ minHeight: '100vh', display: 'flex', flexDirection: 'column' }}>
      <Header />
      <main style={{ flex: 1 }}>
        <Router url={currentPath}>
          <HomePage path="/" />
          <DocsPage path="/docs" />
          <DocsPage path="/docs/:category?" />
          <DocsPage path="/docs/:category/:function?" />
          <PlaygroundPage path="/playground" />
        </Router>
      </main>
    </div>
  )
}