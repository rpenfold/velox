#!/bin/bash

# XL-Formula Code Formatting Script
# Formats all C++ source files using clang-format

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Default values
DRY_RUN=false
VERBOSE=false
CHECK_ONLY=false

# Function to print usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Format C++ source files using clang-format.

OPTIONS:
    -n, --dry-run     Show what would be formatted without making changes
    -c, --check       Check if files are properly formatted (exit 1 if not)
    -v, --verbose     Show verbose output
    -h, --help        Show this help message

EXAMPLES:
    $0                # Format all files
    $0 --dry-run      # Show what would be formatted
    $0 --check        # Check formatting (useful for CI)
    $0 --verbose      # Show detailed output

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -n|--dry-run)
            DRY_RUN=true
            shift
            ;;
        -c|--check)
            CHECK_ONLY=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option $1${NC}"
            usage
            exit 1
            ;;
    esac
done

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}Error: clang-format not found. Please install it:${NC}"
    echo "  macOS: brew install clang-format"
    echo "  Ubuntu: sudo apt-get install clang-format"
    exit 1
fi

# Find all C++ source files
echo -e "${BLUE}Finding C++ source files...${NC}"
CPP_FILES=(
    $(find "$PROJECT_ROOT/cpp" -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | sort)
    $(find "$PROJECT_ROOT/include" -name "*.h" -o -name "*.hpp" 2>/dev/null | sort)
    $(find "$PROJECT_ROOT/tests" -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | sort)
    $(find "$PROJECT_ROOT/examples" -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | sort)
)

if [ ${#CPP_FILES[@]} -eq 0 ]; then
    echo -e "${YELLOW}No C++ files found.${NC}"
    exit 0
fi

echo -e "${BLUE}Found ${#CPP_FILES[@]} C++ files.${NC}"

# Function to check if file needs formatting
needs_formatting() {
    local file="$1"
    local formatted_content=$(clang-format "$file")
    local original_content=$(cat "$file")
    
    if [ "$formatted_content" != "$original_content" ]; then
        return 0  # Needs formatting
    else
        return 1  # Already formatted
    fi
}

# Track statistics
TOTAL_FILES=${#CPP_FILES[@]}
FORMATTED_FILES=0
NEEDS_FORMATTING=()

# Process each file
for file in "${CPP_FILES[@]}"; do
    if [ "$VERBOSE" = true ]; then
        echo -n "Checking $file... "
    fi
    
    if needs_formatting "$file"; then
        NEEDS_FORMATTING+=("$file")
        
        if [ "$CHECK_ONLY" = true ]; then
            if [ "$VERBOSE" = true ]; then
                echo -e "${RED}NEEDS FORMATTING${NC}"
            fi
        elif [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}Would format: $file${NC}"
        else
            if [ "$VERBOSE" = true ]; then
                echo -e "${YELLOW}FORMATTING${NC}"
            fi
            clang-format -i "$file"
            FORMATTED_FILES=$((FORMATTED_FILES + 1))
        fi
    else
        if [ "$VERBOSE" = true ]; then
            echo -e "${GREEN}OK${NC}"
        fi
    fi
done

# Print summary
echo
echo -e "${BLUE}=== FORMATTING SUMMARY ===${NC}"
echo "Total files checked: $TOTAL_FILES"
echo "Files needing formatting: ${#NEEDS_FORMATTING[@]}"

if [ "$CHECK_ONLY" = true ]; then
    if [ ${#NEEDS_FORMATTING[@]} -eq 0 ]; then
        echo -e "${GREEN}✓ All files are properly formatted!${NC}"
        exit 0
    else
        echo -e "${RED}✗ Files need formatting:${NC}"
        for file in "${NEEDS_FORMATTING[@]}"; do
            echo "  - $file"
        done
        echo
        echo -e "${YELLOW}Run '$0' to format these files.${NC}"
        exit 1
    fi
elif [ "$DRY_RUN" = true ]; then
    if [ ${#NEEDS_FORMATTING[@]} -eq 0 ]; then
        echo -e "${GREEN}✓ All files are already properly formatted!${NC}"
    else
        echo -e "${YELLOW}Would format ${#NEEDS_FORMATTING[@]} files.${NC}"
    fi
else
    echo "Files formatted: $FORMATTED_FILES"
    if [ $FORMATTED_FILES -eq 0 ]; then
        echo -e "${GREEN}✓ All files were already properly formatted!${NC}"
    else
        echo -e "${GREEN}✓ Formatting complete!${NC}"
    fi
fi