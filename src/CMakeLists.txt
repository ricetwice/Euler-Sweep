FetchContent_Declare(
        libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG v2.5.0
)

FetchContent_MakeAvailable(libigl)

igl_include(restricted triangle)

add_library(lib_Euler_Sweep EulerOperation.cpp
        Brep.cpp
        utils.cpp
        sweep.cpp
        geometry.cpp)
target_link_libraries(lib_Euler_Sweep igl_restricted::triangle)