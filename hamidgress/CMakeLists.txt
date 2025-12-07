cmake_minimum_required(VERSION 3.20)
project(hamidgress VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

# Build type specific flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Dependencies
find_package(Threads REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Third-party dependencies
# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Project libraries
add_subdirectory(src/common)
add_subdirectory(src/storage)

# Tests
enable_testing()
add_subdirectory(test)

# Main executable (placeholder for now)
add_executable(hamidgress src/main.cpp)
target_link_libraries(hamidgress PRIVATE
    hamidgress_common
    hamidgress_storage
    Threads::Threads
)
