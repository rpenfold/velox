// Utility to handle base path for GitHub Pages
export const getBasePath = () => {
  // Check if we're in production (GitHub Pages)
  if (typeof window !== 'undefined' && window.location.hostname === 'rpenfold.github.io') {
    return '/xl-formula'
  }
  return ''
}

export const getPath = (path) => {
  const basePath = getBasePath()
  return `${basePath}${path}`
} 