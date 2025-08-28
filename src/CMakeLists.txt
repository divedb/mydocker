set(CGROUP_SOURCES
  "cgroup/cgroup.cc"
  "cgroup/memory.cc"
  "cgroup/cgroup_manager.cc"
)

add_library(cgroup STATIC ${CGROUP_SOURCES})
target_link_libraries(cgroup PRIVATE glog::glog)
target_compile_options(cgroup PRIVATE -Wall -Wextra -Wpedantic -g)
target_include_directories(cgroup PRIVATE ${MYDOCKER_INDLUCE_DIR})

set(CMD_SOURCES
  "cmd/cmd.cc"
)

add_library(cmd STATIC ${CMD_SOURCES})
target_link_libraries(cmd PRIVATE CLI11::CLI11)
target_compile_options(cmd PRIVATE -Wall -Wextra -Wpedantic -g)
target_include_directories(cmd PRIVATE ${MYDOCKER_INDLUCE_DIR})

add_executable(mydocker "main/main.cc")
target_link_libraries(mydocker PRIVATE glog::glog CLI11::CLI11 cgroup cmd)
target_compile_options(mydocker PRIVATE -Wall -Wextra -Wpedantic -g)
target_include_directories(mydocker PRIVATE ${MYDOCKER_INDLUCE_DIR})
