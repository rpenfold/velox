#!/usr/bin/env node
// Generate docs overlay from concise Doxygen-style blocks
// Uses:
// - @brief, @ingroup, @param NAME description
// - Optional hints in @param description: (optional), (variadic), (default: X)
// - Examples inside @code ... @endcode, or fallback lines with formula=..., result=..., description=...
// - Optional @name to override Excel name when it differs from C++ function name
// - Optional @see ALIAS to duplicate entries

const fs = require('fs');
const path = require('path');

const ROOT = path.resolve(__dirname, '..');
const CPP_DIR = path.join(ROOT, 'cpp', 'functions');
const OUT_FILE = path.join(ROOT, 'apps', 'docs', 'src', '__generated__', 'functions.js');

function walk(dir) {
  const out = [];
  for (const e of fs.readdirSync(dir)) {
    const p = path.join(dir, e);
    const s = fs.statSync(p);
    if (s.isDirectory()) out.push(...walk(p));
    else if (p.endsWith('.cpp')) out.push(p);
  }
  return out;
}

function parseBlocks(content) {
  const blocks = [];
  const re = /\/\*\*([\s\S]*?)\*\//g;
  let m;
  while ((m = re.exec(content))) {
    const blockText = m[1];
    const endIdx = m.index + m[0].length;
    const lines = blockText.split(/\r?\n/).map(l => l.replace(/^\s*\*\s?/, '').trim());
    const doc = { params: [], examples: [], see: [] };
    let inCode = false; let codeLines = [];
    for (const line of lines) {
      if (line.length === 0) continue;
      if (line.startsWith('@brief')) {
        doc.description = line.replace('@brief', '').trim();
        continue;
      }
      if (line.startsWith('@ingroup')) {
        doc.category = line.replace('@ingroup', '').trim();
        continue;
      }
      if (line.startsWith('@name')) {
        doc.name = line.replace('@name', '').trim();
        continue;
      }
      if (line.startsWith('@param')) {
        const rest = line.replace('@param', '').trim();
        const [name, ...descParts] = rest.split(/\s+/);
        const description = descParts.join(' ').trim();
        const isOptional = /\(optional\)/i.test(description);
        const isVariadic = /\(variadic\)|\.\.\./i.test(description);
        let defaultValue = null;
        const defMatch = description.match(/\(default:\s*([^\)]+)\)/i);
        if (defMatch) defaultValue = defMatch[1].trim();
        doc.params.push({ name, description, required: !isOptional, variadic: !!isVariadic, default: defaultValue });
        continue;
      }
      if (line.startsWith('@see')) {
        const val = line.replace('@see', '').trim();
        if (val) doc.see.push(val);
        continue;
      }
      if (/^@code\b/.test(line)) { inCode = true; codeLines = []; continue; }
      if (/^@endcode\b/.test(line)) {
        // Flush codeLines into examples; try to parse `FORMULA -> RESULT`
        for (const cl of codeLines) {
          const arrow = cl.split(/\s*->\s*/);
          if (arrow.length === 2) {
            doc.examples.push({ formula: arrow[0].trim(), result: arrow[1].trim(), description: '' });
          } else if (/^formula\s*=/.test(cl)) {
            const parts = cl.split(';').map(p => p.trim());
            const ex = {};
            for (const p of parts) {
              const [k, ...rest] = p.split('=');
              if (!k) continue;
              ex[k.trim()] = rest.join('=').trim();
            }
            if (Object.keys(ex).length) doc.examples.push({ formula: ex.formula || '', result: ex.result || '', description: ex.description || '' });
          } else if (cl) {
            doc.examples.push({ formula: cl.trim(), result: '', description: '' });
          }
        }
        inCode = false; codeLines = []; continue;
      }
      if (inCode) { codeLines.push(line); continue; }
      // Fallback old-style example
      if (line.startsWith('@example')) {
        const payload = line.replace('@example', '').trim();
        const parts = payload.split(';').map(p => p.trim());
        const ex = {};
        for (const p of parts) {
          const [k, ...rest] = p.split('=');
          if (!k) continue;
          ex[k.trim()] = rest.join('=').trim();
        }
        doc.examples.push({ formula: ex.formula || '', result: ex.result || '', description: ex.description || '' });
      }
    }
    // Find following function name if @name absent
    if (!doc.name) {
      const tail = content.slice(endIdx);
      const sig = tail.match(/\bValue\s+([A-Za-z0-9_]+)\s*\(/);
      if (sig) {
        const funcName = sig[1];
        // Heuristic mapping: remove trailing _function, uppercase; allow dots via explicit @name where needed
        doc.name = funcName.replace(/_function$/, '').toUpperCase();
      }
    }
    // Accept fallback custom tags if present
    if (!doc.category) {
      const catMatch = blockText.match(/@category\s+([^\n\r]+)/) || blockText.match(/@xl-category\s+([^\n\r]+)/);
      if (catMatch) doc.category = catMatch[1].trim();
    }
    blocks.push(doc);
  }
  return blocks;
}

function buildSyntax(name, params) {
  const items = [];
  let variadic = false;
  for (const p of params) {
    let token = p.name;
    if (p.required === false) token = `[${token}]`;
    items.push(token);
    if (p.variadic) variadic = true;
  }
  let sig = `${name}(${items.join(', ')}`;
  if (variadic) sig += (items.length ? ', ' : '') + '…';
  sig += ')';
  return sig;
}

function addFunction(map, category, name, meta) {
  const catKey = category.toLowerCase();
  if (!map[catKey]) map[catKey] = { functions: {} };
  map[catKey].functions[name] = meta;
}

function ensureCategoryFromPath(filePath) {
  // Expect .../cpp/functions/<category>/...
  const parts = filePath.split(path.sep);
  const idx = parts.lastIndexOf('functions');
  if (idx >= 0 && idx + 1 < parts.length) return parts[idx + 1];
  return 'other';
}

function collectFunctionNames(content) {
  const names = new Set();
  const re = /\bValue\s+([A-Za-z0-9_]+)\s*\(\s*const\s+std::vector<\s*Value\s*>\s*&\s*args/gs;
  let m;
  while ((m = re.exec(content))) {
    names.add(m[1]);
  }
  return Array.from(names);
}

function main() {
  const files = walk(CPP_DIR);
  const overlay = {};
  for (const f of files) {
    if (f.includes(path.join('functions', 'utils') + path.sep)) continue;
    const content = fs.readFileSync(f, 'utf8');
    const docs = parseBlocks(content);
    const categoryFallback = ensureCategoryFromPath(f);
    const docByName = new Map();
    for (const d of docs) {
      if (!d.name || !d.category) continue;
      const syntax = buildSyntax(d.name, d.params || []);
      const meta = {
        name: d.name,
        description: d.description || '',
        syntax,
        parameters: (d.params || []).map(p => ({ name: p.name, description: p.description.replace(/\s*\((optional|variadic|default:[^)]+)\)/ig, '').trim(), required: !!p.required })),
        examples: d.examples || []
      };
      addFunction(overlay, d.category, d.name, meta);
      // Duplicate for @see aliases that look like uppercase function names
      for (const s of d.see || []) {
        if (/^[A-Z0-9_.]+$/.test(s)) {
          addFunction(overlay, d.category, s, meta);
        }
      }
      docByName.set(d.name, true);
    }
    // Add stubs for functions without docblocks so CI/docs always build
    const funcNames = collectFunctionNames(content);
    for (const fn of funcNames) {
      const excelName = fn.replace(/_function$/, '').toUpperCase();
      if (docByName.has(excelName)) continue;
      if (f.includes(path.join('functions', 'utils') + path.sep)) continue;
      // Skip internal helpers commonly used (heuristic)
      if (/^(SINGLE|MULTI|ONE|TWO|NO)_/.test(excelName)) continue;
      const meta = {
        name: excelName,
        description: '',
        syntax: buildSyntax(excelName, []),
        parameters: [],
        examples: []
      };
      addFunction(overlay, categoryFallback, excelName, meta);
    }
  }
  const out = `// AUTO-GENERATED FILE. Do not edit by hand.\nexport const generatedFunctionDocs = ${JSON.stringify(overlay, null, 2)};\n`;
  fs.mkdirSync(path.dirname(OUT_FILE), { recursive: true });
  fs.writeFileSync(OUT_FILE, out, 'utf8');
  console.log(`[generate-docs] Wrote ${OUT_FILE}`);
}

if (require.main === module) {
  main();
}



