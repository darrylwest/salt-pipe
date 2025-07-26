# Master Plan

This document outlines the plan to refactor the `salt-pipe` project.

## 1. Introduce CMake

*   Create a `CMakeLists.txt` file in the project root.
*   Define the project name, C++ standard (c++23), and other basic settings.
*   Add `find_package(sodium REQUIRED)` to locate the libsodium library.
*   The `mk` script will be updated to use `cmake` and `make` for building the project, replacing the direct `g++` call.

## 2. Refactor into a Library and an Executable

*   The core encryption/decryption logic in `src/main.cpp` will be moved into a new file, `src/saltpipe.cpp`, and its header, `include/saltpipe/saltpipe.hpp`.
*   This logic will be placed within the `saltpipe` namespace.
*   The `CMakeLists.txt` will define a `saltpipe` static library containing `src/saltpipe.cpp`.
*   `src/main.cpp` will be updated to be the executable entry point. It will use the `saltpipe` library.
*   The `CMakeLists.txt` will define a `salt-pipe` executable that links against the `saltpipe` library.

## 3. Namespace Refactoring

*   All relevant functions and data structures will be moved into the `saltpipe` namespace.
*   `src/main.cpp` will be updated to use the `saltpipe` namespace.

## 4. Update `mk` script

*   The `build` command in the `mk` script will be changed to use `cmake --build build`.
*   The `all` command will be updated to run `cmake -B build` before building.
*   The direct `g++` compilation line will be removed.
