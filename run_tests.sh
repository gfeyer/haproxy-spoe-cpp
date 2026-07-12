#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

CXX=${CXX:-g++}
CXXFLAGS="-std=c++23 -Wall -Wextra -Iinclude"
BUILD_DIR=$(mktemp -d)
trap 'rm -rf "$BUILD_DIR"' EXIT

status=0

for test_src in tests/*_test.cpp; do
    name=$(basename "$test_src" .cpp)
    bin="$BUILD_DIR/$name"

    echo "== $name =="
    if $CXX $CXXFLAGS "$test_src" -o "$bin" && "$bin"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
        status=1
    fi
done

exit $status
