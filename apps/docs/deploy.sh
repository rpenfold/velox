#!/bin/bash

# Build the app
echo "Building the app..."
npm run build

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "You can now:"
    echo "1. Push to GitHub to trigger automatic deployment"
    echo "2. Or manually upload the 'dist' folder to GitHub Pages"
    echo ""
    echo "To test locally:"
    echo "npx serve dist"
else
    echo "Build failed!"
    exit 1
fi 