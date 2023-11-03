cmake_minimum_required(VERSION 3.26)
project(Euler_Sweep)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

include(FetchContent)

FetchContent_Declare(
        polyscope
        GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
        GIT_TAG        v1.3.0
)

# CMake 3.14+
FetchContent_MakeAvailable(polyscope)

include_directories(include)
add_subdirectory(src)
add_executable(Euler_Sweep main.cpp)
target_link_libraries(Euler_Sweep lib_Euler_Sweep)
target_link_libraries(Euler_Sweep polyscope)
