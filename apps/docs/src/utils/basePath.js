// Base path utility - uses environment variable set at build time
const BASE_PATH = import.meta.env.VITE_BASE_PATH || '/'

export const getBasePath = () => {
  return BASE_PATH
}

export const getPath = (path) => {
  return `${BASE_PATH}${path}`
}

// Get the current path relative to the base path
export const getCurrentPath = () => {
  if (typeof window !== 'undefined') {
    const basePath = getBasePath()
    const pathname = window.location.pathname
    return pathname.replace(basePath, '') || '/'
  }
  return '/'
} 