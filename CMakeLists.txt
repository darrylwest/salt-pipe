cmake_minimum_required(VERSION 3.22)
project(salt-pipe VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM.cmake to manage dependencies
set(CPM_DOWNLOAD_VERSION 0.38.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}...")
    file(DOWNLOAD
         "https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
         "${CPM_DOWNLOAD_LOCATION}")
endif()
include(${CPM_DOWNLOAD_LOCATION})

# Add libsodium dependency using CPM
CPMAddPackage(
    NAME libsodium
    GITHUB_REPOSITORY "jedisct1/libsodium"
    GIT_TAG "stable"
    OPTIONS "SODIUM_DISABLE_TESTS ON" "BUILD_SHARED_LIBS OFF"
)

# Define the saltpipe static library
add_library(saltpipe STATIC src/saltpipe.cpp)

# Make include directory available
target_include_directories(saltpipe PUBLIC include)

# Link libsodium to the saltpipe library
target_link_libraries(saltpipe PUBLIC sodium)

# Define the main executable
add_executable(salt-pipe src/main.cpp)

# Link the executable against our library
target_link_libraries(salt-pipe PRIVATE saltpipe)
