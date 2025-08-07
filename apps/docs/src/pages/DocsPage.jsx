import { useState, useEffect } from 'preact/hooks'
import { Link, route } from 'preact-router'
import { functionCategories, getAllFunctions } from '../data/functions.js'
import { FunctionDetail } from '../components/FunctionDetail.jsx'

export function DocsPage({ category, function: functionName }) {
  // Use URL parameters as the single source of truth
  const selectedCategory = category || 'all'
  const selectedFunction = functionName || null
  const [searchTerm, setSearchTerm] = useState('')

  // Redirect to default category if none specified and we're at base /docs
  useEffect(() => {
    if (!category && !functionName && window.location.pathname === '/docs') {
      route('/docs/all', true)
    }
  }, [category, functionName])

  const currentCategory = functionCategories[selectedCategory]
  const currentFunction = selectedFunction && currentCategory?.functions[selectedFunction.toUpperCase()]

  // Filter functions based on search term and sort alphabetically
  const getFilteredFunctions = () => {
    if (!currentCategory) return []
    
    const functionsToFilter = Object.entries(currentCategory.functions)
    
    let filtered
    if (!searchTerm.trim()) {
      filtered = functionsToFilter
    } else {
      const searchLower = searchTerm.toLowerCase()
      filtered = functionsToFilter.filter(([key, func]) => 
        func.name.toLowerCase().includes(searchLower) ||
        func.description.toLowerCase().includes(searchLower) ||
        func.syntax.toLowerCase().includes(searchLower)
      )
    }
    
    // Sort alphabetically by function name
    return filtered.sort(([keyA, funcA], [keyB, funcB]) => 
      funcA.name.localeCompare(funcB.name)
    )
  }

  const filteredFunctions = getFilteredFunctions()

  if (selectedFunction && currentFunction) {
    return (
      <div className="container" style={{ padding: '2rem 0' }}>
        <div className="mb-6">
          <nav className="flex items-center gap-2 text-sm text-muted">
            <Link 
              href="/docs" 
              style={{ textDecoration: 'none', color: 'var(--color-primary)' }}
            >
              Documentation
            </Link>
            <span>→</span>
            <Link 
              href={`/docs/${selectedCategory}`}
              style={{ textDecoration: 'none', color: 'var(--color-primary)' }}
            >
              {currentCategory?.name}
            </Link>
            <span>→</span>
            <span>{currentFunction.name}</span>
          </nav>
        </div>
        
        <FunctionDetail 
          func={currentFunction} 
          category={selectedCategory}
          categoryName={currentCategory.name}
        />
      </div>
    )
  }

  return (
    <div className="container" style={{ padding: '2rem 0' }}>
      <h1 className="text-2xl font-bold mb-6">Documentation</h1>
      
      <div className="grid grid-cols-4 gap-6">
        {/* Sidebar */}
        <div className="card">
          <h3 className="font-semibold mb-4">Categories</h3>
          
          <div className="grid gap-2">
            {Object.entries(functionCategories).map(([key, cat]) => (
              <Link
                key={key}
                href={`/docs/${key}`}
                className={`p-3 rounded text-sm ${selectedCategory === key ? 'bg-primary text-white' : 'hover:bg-secondary'}`}
                style={{ 
                  textDecoration: 'none',
                  display: 'block',
                  background: selectedCategory === key ? 'var(--color-primary)' : 'transparent',
                  color: selectedCategory === key ? 'white' : 'var(--color-text)'
                }}
              >
                <div className="font-medium">{cat.name}</div>
                <div className="text-xs opacity-80">
                  {Object.keys(cat.functions).length} functions
                </div>
              </Link>
            ))}
          </div>
        </div>

        {/* Function List - spans 3 columns */}
        <div className="card col-span-3">
          <div className="flex justify-between items-center mb-4">
            <h3 className="font-semibold">
              {currentCategory?.name}
            </h3>
            <div className="text-sm text-muted">
              {filteredFunctions.length} function{filteredFunctions.length !== 1 ? 's' : ''}
              {searchTerm && ` matching "${searchTerm}"`}
            </div>
          </div>
          
          {/* Search Bar */}
          <div className="mb-4">
            <input
              type="text"
              placeholder="Search functions..."
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              onInput={(e) => setSearchTerm(e.target.value)}
              onKeyUp={(e) => setSearchTerm(e.target.value)}
              style={{
                width: '100%',
                padding: '8px 12px',
                border: '1px solid var(--color-border)',
                borderRadius: '4px',
                fontSize: '14px'
              }}
            />
          </div>
          
          <p className="text-sm text-muted mb-4">
            {currentCategory?.description}
          </p>
          
          {/* Two column function grid */}
          <div className="grid grid-cols-2 gap-3">
            {filteredFunctions.length > 0 ? filteredFunctions.map(([key, func]) => (
              <Link
                key={key}
                href={`/docs/${selectedCategory}/${key.toLowerCase()}`}
                className="p-3 rounded border hover:border-primary"
                style={{ 
                  textDecoration: 'none',
                  display: 'block',
                  border: '1px solid var(--color-border)'
                }}
              >
                <div className="font-mono font-medium text-sm">{func.name}</div>
                <div className="text-xs text-muted mt-1">{func.description}</div>
              </Link>
            )) : (
              <div className="col-span-2 text-center py-8 text-muted">
                <div className="text-lg mb-2">No functions found</div>
                <div className="text-sm">
                  {searchTerm ? `No functions match "${searchTerm}"` : 'No functions in this category'}
                </div>
              </div>
            )}
          </div>
        </div>
      </div>
    </div>
  )
}