#!/usr/bin/env node
const fs = require('fs');
const path = require('path');

(async () => {
  const ROOT = path.resolve(__dirname, '..');
  const manualPath = path.join(ROOT, 'apps', 'docs', 'src', 'data', 'functions.js');
  const generatedPath = path.join(ROOT, 'apps', 'docs', 'src', '__generated__', 'functions.js');

  // Ensure generated is up to date
  try {
    // eslint-disable-next-line global-require, import/no-dynamic-require
    require('./generate-docs.js');
  } catch (_) {
    // ignore; script may not be commonjs
  }

  const manualSrc = fs.readFileSync(manualPath, 'utf8');
  const manualFns = new Set();
  // Rough extraction: capture uppercase function keys inside object literals
  const fnRegex = /(^|\n)\s*(?:['"])??([A-Z0-9_.]+)(?:['"])??\s*:\s*\{/g;
  let m;
  while ((m = fnRegex.exec(manualSrc))) {
    // Exclude likely non-function keys (very short or ALL) if needed
    const key = m[2];
    if (key && key === key.toUpperCase() && key.length >= 2) {
      manualFns.add(key);
    }
  }

  // Load generated overlay via file URL to support ESM
  const fileUrl = 'file://' + generatedPath;
  const mod = await import(fileUrl);
  const gen = mod.generatedFunctionDocs || {};
  const genFns = new Set();
  for (const catObj of Object.values(gen)) {
    if (!catObj || !catObj.functions) continue;
    for (const fnName of Object.keys(catObj.functions)) genFns.add(fnName);
  }

  const onlyInManual = [...manualFns].filter((x) => !genFns.has(x)).sort();
  const onlyInGenerated = [...genFns].filter((x) => !manualFns.has(x)).sort();

  console.log('manual_count', manualFns.size);
  console.log('generated_count', genFns.size);
  console.log('only_in_manual_count', onlyInManual.length);
  console.log('only_in_manual_sample', onlyInManual.slice(0, 100));
  console.log('only_in_generated_count', onlyInGenerated.length);
  console.log('only_in_generated_sample', onlyInGenerated.slice(0, 100));
})();


