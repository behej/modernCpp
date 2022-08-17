cmake_minimum_required(VERSION 3.5)


project(modernCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)            # Declare which C++ version to use. Can be 98, 11, 14, 17 (CMake >3.8), 20 (CMake >3.12)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Requires C++ standard to be applied. CMake doesn't downgrade is no compatible compiler is found. (Default is OFF)
set(CMAKE_CXX_EXTENSIONS OFF)         # Disables compiler specific extensions. may stick to option -std=c++11 instead of -std=gnu++11. Recommended for broader platforms compatibility (Default is ON)

add_executable(hello main.cpp)
add_executable(constructors constructors.cpp)
