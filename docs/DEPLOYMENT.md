# Deploying Velox Docs to GitHub Pages

This guide explains how to deploy the docs/playground app to GitHub Pages.

## Option 1: Automatic Deployment (Recommended)

### Setup GitHub Actions

1. **Push the workflow file**: The `.github/workflows/deploy.yml` file will automatically deploy your app when you push to the `main` branch.

2. **Enable GitHub Pages**:
   - Go to your repository on GitHub
   - Navigate to **Settings** → **Pages**
   - Under **Source**, select **Deploy from a branch**
   - Choose **gh-pages** branch and **/(root)** folder
   - Click **Save**

3. **Push your changes**:
   ```bash
   git add .
   git commit -m "Add GitHub Pages deployment"
   git push origin main
   ```

4. **Wait for deployment**: GitHub Actions will automatically build and deploy your app. You can monitor the progress in the **Actions** tab.

### Your app will be available at
`https://[your-username].github.io/<repo-name>/`

## Option 2: Manual Deployment

If you prefer manual deployment:

1. **Build the app locally**:
   ```bash
   cd apps/docs
   npm run build
   ```

2. **Upload to GitHub Pages**:
   - Go to your repository on GitHub
   - Navigate to **Settings** → **Pages**
   - Under **Source**, select **Deploy from a branch**
   - Choose your main branch and `/apps/docs/dist` folder
   - Click **Save**

## Option 3: Using GitHub CLI

You can also deploy using GitHub CLI:

```bash
# Install GitHub CLI if you haven't already
# brew install gh (on macOS)

# Login to GitHub
gh auth login


## Local Testing

To test the built app locally:

```bash
cd apps/docs
npm run build
npx serve dist
```

Then visit `http://localhost:3000`

## Troubleshooting

### Common Issues:

1. **404 errors**: Make sure the `base` path in `vite.config.js` matches your repository name
2. **WASM not loading**: Ensure the WASM files are included in the build
3. **Routing issues**: If using client-side routing, you may need to configure a 404.html redirect

### Custom Domain (Optional)

If you want to use a custom domain:

1. Add a `CNAME` file to the `apps/docs/public` folder with your domain
2. Configure your DNS settings
3. Update the `base` path in `vite.config.js` to `/`

## Environment Variables

The app automatically detects the environment:
- **Development**: Uses `/` as base path
- **Production**: Set `VITE_BASE_PATH` to `/<repo-name>/` if deploying to a subpath

## Notes

- GitHub Pages is completely free
- Your app will be publicly accessible
- Updates are automatic when you push to main
- The `gh-pages` branch will be created automatically by the GitHub Action 