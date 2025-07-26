#!/usr/bin/env bash
#
# s3runner build script
# 2025-07-25 dpw
#

set -eu

export project=salt-pipe

export verbose=''
export os="$(uname -s)"
export arch="$(uname -m)"

[ $os == "Linux" ] && {
    # export CC=/usr/local/bin/gcc
    # export CXX=/usr/local/bin/g++
    export FLAGS="-j8"
} || {
    export FLAGS="-j20"
}

# parse the cli
while [[ $# -gt 0 ]]
do
    case $1 in
        verbose)
            verbose='VERBOSE=1'

            shift
        ;;
        all)
            echo "pull latest code from `git branch --show-current` branch."
            git pull

            /bin/rm -fr build && cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON && cmake --build build

            exit 0
        ;;
        init)
            [ -d build ] || mkdir build
            cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON

            shift
        ;;
        build)
            [ -d build ] || cmake -B build
            cmake --build build

            shift
        ;;
        unit|test)
            # build it first
            cmake --build build

            echo "Running unit tests..."
            ./build/unit_tests

            shift
        ;;
        run)
            ./build/salt-pipe

            shift
        ;;
        version)
            ./build/$project --version

            shift
        ;;
        format)
            find src include tests -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
            echo "✅ Code formatting applied"
            git status -s

            shift
        ;;
        clean)
            (cd build && make clean)
        
            shift
        ;;
        clob*)
            /bin/rm -fr build/

            shift
        ;;
        watch)
            watchexec -c -w src/ -w include/ -w tests/ -e h,hpp,cpp ./mk build test

            exit 0
        ;;
        pull)
            git pull

            shift
        ;;

        help)
            echo "s3runner build script targets:"
            echo ""
            echo "   init     : run the cmake command to create the build folder"
            echo "   build    : compile s3run to the build folder"
            echo "   test     : run the unit tests"
            echo "   version  : show s3run version"
            echo "   format   : runs clang-format over includes and src"
            echo "   run      : runs an example"
            echo "   watch    : run watcher over source, include and unit tests"
            echo "   pull     : pull the latest repo changes"
            echo "   clean    : remove binary builds but leave the build folder"
            echo "   clobber  : remove the entire build folder"
            echo "   all      : special target to pull latest, clobber, init, build"
            echo "   verbose  : show a verbose build"
            echo "   help     : show this help"
            
            exit 0
        ;;
        *)
            ./mk help
            exit 0
        ;;

    esac
done

