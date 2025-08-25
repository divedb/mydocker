set(SOURCES
  "cgroup/memory.cc"
  "cgroup/cgroup_manager.cc"
)

add_library(mydocker STATIC ${SOURCES})
target_link_libraries(mydocker PRIVATE glog::glog)
target_compile_options(mydocker PRIVATE -Wall -Wextra -g)
target_include_directories(mydocker PRIVATE ${MYDOCKER_INDLUCE_DIR})
