// Base path utility - uses environment variable set at build time for assets
const BASE_PATH = import.meta.env.VITE_BASE_PATH || '/'

// Runtime detection for routing
let cachedRuntimeBasePath = null

export const getBasePath = () => {
  return BASE_PATH
}

export const getRuntimeBasePath = () => {
  if (cachedRuntimeBasePath !== null) {
    return cachedRuntimeBasePath
  }

  if (typeof window !== 'undefined') {
    const pathname = window.location.pathname
    // Check if we're in a subdirectory deployment
    if (pathname.startsWith('/xl-formula')) {
      cachedRuntimeBasePath = '/xl-formula'
    } else {
      cachedRuntimeBasePath = ''
    }
  } else {
    cachedRuntimeBasePath = ''
  }

  return cachedRuntimeBasePath
}

export const getPath = (path) => {
  return `${BASE_PATH}${path}`
}

// Get the current path relative to the runtime base path
export const getCurrentPath = () => {
  if (typeof window !== 'undefined') {
    const runtimeBasePath = getRuntimeBasePath()
    const pathname = window.location.pathname
    return pathname.replace(runtimeBasePath, '') || '/'
  }
  return '/'
} 