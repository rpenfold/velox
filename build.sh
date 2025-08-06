#!/bin/bash

# XL-Formula Build Script
# Supports building the library, tests, and examples

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Default values
BUILD_TYPE="Release"
BUILD_TESTS=ON
BUILD_EXAMPLES=ON
BUILD_WEB=OFF
BUILD_RN=OFF
CLEAN=false
RUN_TESTS=false
INSTALL=false
FORMAT_CODE=false
FORMAT_CHECK=false

# Help function
show_help() {
    cat << EOF
XL-Formula Build Script

Usage: $0 [OPTIONS]

Options:
    -h, --help          Show this help message
    -d, --debug         Build in Debug mode (default: Release)
    -c, --clean         Clean build directory before building
    -t, --tests         Build and run tests
    -e, --examples      Build examples (default: ON)
    --no-tests          Don't build tests
    --no-examples       Don't build examples
    --web               Build web bindings (not implemented)
    --react-native      Build React Native bindings (not implemented)
    --install           Install after building
    --coverage          Build with coverage information
    --format            Format all C++ source files with clang-format
    --format-check      Check if files are properly formatted (exit 1 if not)

Examples:
    $0                  # Basic release build
    $0 -d -t            # Debug build with tests
    $0 -c --coverage    # Clean build with coverage
    $0 --format         # Format all code

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -t|--tests)
            RUN_TESTS=true
            shift
            ;;
        -e|--examples)
            BUILD_EXAMPLES=ON
            shift
            ;;
        --no-tests)
            BUILD_TESTS=OFF
            shift
            ;;
        --no-examples)
            BUILD_EXAMPLES=OFF
            shift
            ;;
        --web)
            BUILD_WEB=ON
            shift
            ;;
        --react-native)
            BUILD_RN=ON
            shift
            ;;
        --install)
            INSTALL=true
            shift
            ;;
        --coverage)
            BUILD_TYPE="Coverage"
            shift
            ;;
        --format)
            FORMAT_CODE=true
            shift
            ;;
        --format-check)
            FORMAT_CHECK=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

# Handle format operations first (before building)
if [[ "$FORMAT_CODE" == "true" ]]; then
    print_status "Formatting C++ source files..."
    if [[ -x "$SCRIPT_DIR/scripts/format.sh" ]]; then
        "$SCRIPT_DIR/scripts/format.sh" --verbose
    else
        print_error "Format script not found or not executable: $SCRIPT_DIR/scripts/format.sh"
        exit 1
    fi
    exit 0
fi

if [[ "$FORMAT_CHECK" == "true" ]]; then
    print_status "Checking C++ source file formatting..."
    if [[ -x "$SCRIPT_DIR/scripts/format.sh" ]]; then
        "$SCRIPT_DIR/scripts/format.sh" --check --verbose
    else
        print_error "Format script not found or not executable: $SCRIPT_DIR/scripts/format.sh"
        exit 1
    fi
    exit 0
fi

print_status "XL-Formula Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Build Tests: $BUILD_TESTS"
echo "  Build Examples: $BUILD_EXAMPLES"
echo "  Build Web: $BUILD_WEB"
echo "  Build React Native: $BUILD_RN"
echo "  Clean: $CLEAN"
echo "  Run Tests: $RUN_TESTS"
echo "  Install: $INSTALL"
echo ""

# Clean if requested
if [[ "$CLEAN" == "true" ]]; then
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
print_status "Configuring with CMake..."
CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DBUILD_TESTS="$BUILD_TESTS"
    -DBUILD_EXAMPLES="$BUILD_EXAMPLES"
    -DBUILD_WEB_BINDINGS="$BUILD_WEB"
    -DBUILD_RN_BINDINGS="$BUILD_RN"
)

# Special handling for web builds
if [[ "$BUILD_WEB" == "ON" ]]; then
    if ! command -v emcmake &> /dev/null; then
        print_error "Emscripten not found. Please install Emscripten for web builds."
        exit 1
    fi
    print_status "Using Emscripten for web build..."
    emcmake cmake "${CMAKE_ARGS[@]}" "$SCRIPT_DIR"
else
    cmake "${CMAKE_ARGS[@]}" "$SCRIPT_DIR"
fi

# Build
print_status "Building..."
if [[ "$BUILD_WEB" == "ON" ]]; then
    emmake make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
else
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

print_status "Build completed successfully!"

# Run tests if requested
if [[ "$RUN_TESTS" == "true" && "$BUILD_TESTS" == "ON" ]]; then
    print_status "Running tests..."
    if [[ "$BUILD_WEB" == "ON" ]]; then
        print_warning "Cannot run tests automatically for web builds"
    else
        ctest --output-on-failure
        
        # Generate coverage report if in coverage mode
        if [[ "$BUILD_TYPE" == "Coverage" ]]; then
            print_status "Generating coverage report..."
            if command -v gcovr &> /dev/null; then
                make coverage
                print_status "Coverage report generated: coverage.html"
            else
                print_warning "gcovr not found. Install it to generate coverage reports."
            fi
        fi
    fi
fi

# Install if requested
if [[ "$INSTALL" == "true" ]]; then
    print_status "Installing..."
    make install
fi

# Show example commands
echo ""
print_status "Build complete! Here are some things you can do:"

if [[ "$BUILD_EXAMPLES" == "ON" && "$BUILD_WEB" != "ON" ]]; then
    echo "  Run basic example:    ./examples/basic_example"
    echo "  Run advanced example: ./examples/advanced_example"
fi

if [[ "$BUILD_TESTS" == "ON" && "$BUILD_WEB" != "ON" ]]; then
    echo "  Run tests:            ctest"
    echo "  Run specific test:    ./xl-formula-tests --gtest_filter=*TestName*"
fi

if [[ "$BUILD_TYPE" == "Coverage" ]]; then
    echo "  View coverage:        open coverage.html"
fi

echo ""
print_status "Build script completed successfully!"