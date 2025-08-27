# VeloxPlayground

A React Native app for testing and experimenting with the Velox formula engine in a mobile environment.

## Features

- Formula evaluation testing
- Real-time formula input and validation
- Performance monitoring
- Cross-platform compatibility (iOS/Android)
- **Full native module support** for custom C++ integration

## Native Module Support

This app is built with **pure React Native CLI** (not Expo), which provides:

✅ **Full access to native modules**  
✅ **Custom C++ integration**  
✅ **Direct native code modification**  
✅ **Proprietary module support**  
✅ **Complete control over native dependencies**

## Prerequisites

- Node.js 18 or later
- React Native CLI
- Xcode (for iOS development)
- Android Studio (for Android development)

## Installation

This app is part of the Velox monorepo. For the simplest setup, use the root-level scripts:

```bash
# From the project root
npm ci                    # Install all dependencies
npm run mobile:prepare    # Install iOS pods
npm run mobile:start      # Start Metro bundler
npm run mobile:ios        # Run on iOS simulator
```

## Running the App

### From Project Root (Recommended)
```bash
# Start Metro bundler
npm run mobile:start

# Run on iOS simulator
npm run mobile:ios

# Run on Android emulator
npm run mobile:android

# Start Metro and run iOS simultaneously
npm run mobile:dev
```

### From This Directory
```bash
# Start Metro bundler
npm start

# Run on iOS simulator
npm run ios

# Run on Android emulator
npm run android
```

## Development

### Project Structure
```
├── android/           # Android native code
├── ios/              # iOS native code
├── App.tsx           # Main app component
├── index.js          # Entry point
├── package.json      # Dependencies and scripts
└── README.md         # This file
```

### Available Scripts

#### Root Level Scripts (Recommended)
- `npm run mobile:setup` - Complete setup (npm ci + pod install)
- `npm run mobile:prepare` - Install iOS CocoaPods
- `npm run mobile:start` - Start Metro bundler on port 8082
- `npm run mobile:ios` - Build and run on iOS simulator
- `npm run mobile:android` - Build and run on Android emulator
- `npm run mobile:dev` - Start Metro and run iOS app simultaneously
- `npm run mobile:clean` - Clean build artifacts and reinstall pods

#### Local Scripts
- `npm start` - Start Metro bundler
- `npm run ios` - Run on iOS simulator
- `npm run android` - Run on Android emulator
- `npm test` - Run tests
- `npm run lint` - Run ESLint
- `npm run pod-install` - Install iOS pods

## Integration with Velox

This app is designed to integrate with the Velox formula engine. Future updates will include:

- Direct integration with the C++ formula engine
- Real-time formula evaluation
- Performance benchmarking
- Error handling and validation
- Custom native modules for formula processing

## Native Module Development

To add custom native modules:

### iOS
- Add native code to `ios/` directory
- Create Objective-C/Swift bridges
- Link with C++ formula engine

### Android
- Add native code to `android/` directory
- Create Java/Kotlin bridges
- Link with C++ formula engine via JNI

## Contributing

1. Follow the TypeScript coding standards
2. Run tests before submitting changes
3. Ensure code is properly formatted with Prettier
4. Follow the existing code structure and patterns
5. Test on both iOS and Android platforms
