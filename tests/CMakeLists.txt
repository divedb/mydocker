set(TESTS
  cgroup_version_test
  cgroup_manager_test
)

foreach(test_name IN LISTS TESTS)
  add_executable(${test_name} ${test_name}.cc)
  target_include_directories(${test_name} PRIVATE ${MYDOCKER_INDLUCE_DIR})
  target_link_libraries(${test_name} PRIVATE ${GTEST_LIBRARIES} pthread mydocker)
  target_compile_options(${test_name} PRIVATE -Wall -Wextra -g)
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
