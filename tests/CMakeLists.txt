include(FetchContent)

# 描述如何获取 Catch2
FetchContent_Declare(
        catch2 # 建议使用全小写
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0)

# 一条龙地下载、构建 Catch2
FetchContent_MakeAvailable(catch2)

add_executable(testEuler testEuler.cpp)
add_test(NAME Sweep.testEuler COMMAND testEuler)
target_link_libraries(testEuler PRIVATE lib_Euler_Sweep)
target_link_libraries(testEuler PRIVATE Catch2::Catch2WithMain)
target_link_libraries(testEuler PRIVATE polyscope)