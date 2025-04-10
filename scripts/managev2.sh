#!/bin/bash
script_dir=$(dirname "$0")

# Function to detect the current platform
get_platform() {
    case "$(uname -s)" in
        Linux*)     echo "linux";;
        Darwin*)    echo "macos";;
        CYGWIN*|MINGW*|MSYS*) echo "windows";;
        *)          echo "unknown";;
    esac
}

# Get the current platform
CURRENT_PLATFORM=$(get_platform)

# Build function: Configures and builds the project for the current platform
build() {
    case "$CURRENT_PLATFORM" in
        linux|macos)
            # Single-configuration build (Release specified at configure time)
            cmake -B build-$CURRENT_PLATFORM -S . -DCMAKE_BUILD_TYPE=Release
            cmake --build build-$CURRENT_PLATFORM
            ;;
        windows)
            # Multi-configuration build (Release specified at build time)
            cmake -B build-$CURRENT_PLATFORM -S .
            cmake --build build-$CURRENT_PLATFORM --config Release
            ;;
        *)
            echo "Unsupported platform: $CURRENT_PLATFORM"
            exit 1
            ;;
    esac
}

# Package function: Creates packages using CPack
package() {
    if [ "$CURRENT_PLATFORM" = "windows" ]; then
        # For multi-config generators (e.g., Visual Studio), specify Release
        cpack -C Release --config build-$CURRENT_PLATFORM/CPackConfig.cmake
    else
        # For single-config generators (e.g., Make), no config needed
        cpack --config build-$CURRENT_PLATFORM/CPackConfig.cmake
    fi
}

# Release function: Builds and packages in one go
release() {
    build
    package
}

# Handle script arguments
case "$1" in
    build)
        build
        ;;
    package)
        package
        ;;
    release)
        release
        ;;
    clean)
        rm -rf build-*
        rm -rf install
        rm -rf _CPack_Packages
        rm -rf Packaging
        ;;
    test)
        build
        cd build-$CURRENT_PLATFORM
        ctest --test-dir build-$CURRENT_PLATFORM
        ;;
    gen_docs)
        doxygen $script_dir/../Doxyfile
        ;;
    open_docs)
        open $script_dir/../docs/html/index.html
        ;;
    *)
        echo "Usage: $0 {build|package|release|clean|test|gen_docs|open_docs}"
        exit 1
        ;;
esac