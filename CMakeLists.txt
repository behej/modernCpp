cmake_minimum_required(VERSION 3.5)


project(modernCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)            # Declare which C++ version to use. Can be 98, 11, 14, 17 (CMake >3.8), 20 (CMake >3.12)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Requires C++ standard to be applied. CMake doesn't downgrade if no compatible compiler is found. (Default is OFF)
set(CMAKE_CXX_EXTENSIONS OFF)         # Disables compiler specific extensions. may stick to option -std=c++11 instead of -std=gnu++11. Recommended for broader platforms compatibility (Default is ON)

# Basic hello world
add_executable(hello main.cpp)
# New stuff about constructors: default, delete, explicit, NSDMI
add_executable(constructors constructors.cpp)
# Initializatons: braces init, auto, decltype, const and constexpr
add_executable(initializations initializations.cpp)
# Move semantic: Base principles for move semantics for values and for classes
add_executable(moveSemantic moveSemantic.cpp)
# Smart pointers: unique_ptr and shared_ptr
add_executable(smartPointers smartPointers.cpp)
# Containers
add_executable(containers containers.cpp)

add_executable(sandbox sandbox.cpp)
# iterators.cpp
