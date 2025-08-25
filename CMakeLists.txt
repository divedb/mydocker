cmake_minimum_required(VERSION 3.20)
project(mydocker)

set(CMAKE_CXX_STANDARD 20)
set(MYDOCKER_INDLUCE_DIR ${CMAKE_SOURCE_DIR}/include)
enable_testing()


include(FetchContent)
FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)

FetchContent_MakeAvailable(cli11_proj)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(glog REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)