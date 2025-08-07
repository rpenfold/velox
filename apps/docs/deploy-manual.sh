#!/bin/bash

echo "=== Manual GitHub Pages Deployment ==="

# Build the app
echo "Building the app..."
npm run build

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo ""
    echo "📁 Built files are in: apps/docs/dist/"
    echo ""
    echo "🔧 To deploy manually:"
    echo "1. Go to your repository on GitHub"
    echo "2. Settings → Pages"
    echo "3. Source: 'Deploy from a branch'"
    echo "4. Branch: 'main'"
    echo "5. Folder: '/apps/docs/dist'"
    echo "6. Save"
    echo ""
    echo "🌐 Your app will be available at:"
    echo "https://[your-username].github.io/xl-formula/"
    echo ""
    echo "📝 Note: You may need to wait a few minutes for the first deployment."
else
    echo "❌ Build failed!"
    exit 1
fi 