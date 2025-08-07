import { Router } from 'preact-router'
import { Header } from './components/Header.jsx'
import { HomePage } from './pages/HomePage.jsx'
import { DocsPage } from './pages/DocsPage.jsx'
import { PlaygroundPage } from './pages/PlaygroundPage.jsx'

export function App() {
  return (
    <div style={{ minHeight: '100vh', display: 'flex', flexDirection: 'column' }}>
      <Header />
      <main style={{ flex: 1 }}>
        <Router>
          <HomePage path="/" />
          <HomePage path="/xl-formula" />
          <HomePage path="/xl-formula/" />
          <DocsPage path="/docs" />
          <DocsPage path="/xl-formula/docs" />
          <DocsPage path="/docs/:category?" />
          <DocsPage path="/xl-formula/docs/:category?" />
          <DocsPage path="/docs/:category/:function?" />
          <DocsPage path="/xl-formula/docs/:category/:function?" />
          <PlaygroundPage path="/playground" />
          <PlaygroundPage path="/xl-formula/playground" />
        </Router>
      </main>
    </div>
  )
}