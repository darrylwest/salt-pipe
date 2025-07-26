cmake_minimum_required(VERSION 3.22)
project(salt-pipe VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    libsodium
    GIT_REPOSITORY https://github.com/jedisct1/libsodium.git
    GIT_TAG stable
)
FetchContent_MakeAvailable(libsodium)

option(ENABLE_TESTS "Build tests" ON)

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

if(ENABLE_TESTS)
  # Add testing logic here in the future
endif()
