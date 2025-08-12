import { generatedFunctionDocs } from '../__generated__/functions.js'

// Build categories directly from generated docs
export const functionCategories = {}
Object.entries(generatedFunctionDocs || {}).forEach(([catKey, catVal]) => {
  functionCategories[catKey] = {
    name: catKey.charAt(0).toUpperCase() + catKey.slice(1),
    description: '',
    functions: { ...(catVal?.functions || {}) }
  }
})

// Add synthetic "all" category
functionCategories.all = {
  name: 'All Functions',
  description: 'All available functions across all categories',
  functions: {}
}

export const getAllFunctions = () => {
  const allFunctions = []
  Object.entries(functionCategories).forEach(([categoryKey, category]) => {
    if (categoryKey === 'all') return
    Object.entries(category.functions).forEach(([fnKey, meta]) => {
      allFunctions.push({ ...meta, category: categoryKey, categoryName: category.name })
    })
  })
  return allFunctions
}

export const populateAllCategory = () => {
  const allFunctions = getAllFunctions()
  const map = {}
  allFunctions.forEach(f => { map[f.name] = f })
  functionCategories.all.functions = map
  return functionCategories
}

populateAllCategory()


