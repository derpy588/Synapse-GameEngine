#!/bin/bash
script_dir=$(dirname "$0")

case "$1" in
    build)
        mkdir -p $script_dir/../build
        cmake -S . -B $script_dir/../build
        cmake --build $script_dir/../build
        ;;
    test)
        $script_dir/manage.sh build  # Reuse the build command
        cd build
        ctest
        ;;
    docs)
        doxygen $script_dir/../Doxyfile
        ;;
    clean)
        $script_dir/clean.sh
        ;;
    opendocs)
        open $script_dir/../docs/html/index.html
        ;;
    package)
        $script_dir/manage.sh build  # Reuse the build command
        cmake --install $script_dir/../build --prefix install
        cpack --config $script_dir/../build/CPackConfig.cmake
        ;;
    *)
        echo "Usage: $0 {build|test|docs|clean|opendocs|package|setup}"
        exit 1
        ;;
esac