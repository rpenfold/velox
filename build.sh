#!/bin/bash
echo "This script has moved to scripts/build.sh" >&2
exec "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/scripts/build.sh" "$@"