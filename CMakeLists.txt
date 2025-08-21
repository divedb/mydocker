cmake_minimum_required(VERSION 3.20)
project(mydocker)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

add_executable(cgroup_version_test test/cgroup_version_test.cc)
target_link_libraries(cgroup_version_test PRIVATE ${GTEST_LIBRARIES} pthread)
add_test(NAME CgroupVersionGTest COMMAND cgroup_version_test)