const {getDefaultConfig, mergeConfig} = require('@react-native/metro-config');
const path = require('path');

/**
 * Metro configuration for React Native in monorepo
 * https://facebook.github.io/metro/docs/configuration
 *
 * @type {import('metro-config').MetroConfig}
 */
const config = {
    server: { port: 8082 },
    resolver: {
        // Enable symlinks for monorepo
        unstable_enableSymlinks: true,
        // Add node_modules paths for monorepo
        nodeModulesPaths: [
            path.resolve(__dirname, 'node_modules'),
            path.resolve(__dirname, '../../node_modules'),
        ],
        // Disable hierarchical lookup to prevent issues
        disableHierarchicalLookup: true,
    },
    // Watch folders for monorepo
    watchFolders: [
        path.resolve(__dirname, '../../node_modules'),
    ],
};

module.exports = mergeConfig(getDefaultConfig(__dirname), config);
