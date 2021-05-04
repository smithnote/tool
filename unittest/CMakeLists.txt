include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/lib)

add_executable(thread_pool_unittest thread_pool_unittest.cpp)
add_dependencies(thread_pool_unittest deps)
target_link_libraries(thread_pool_unittest thread_pool gtest pthread)
install(TARGETS thread_pool_unittest DESTINATION bin)
add_test(NAME unittest_thread_pool COMMAND thread_pool_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(connection_pool_unittest connection_pool_unittest.cpp)
add_dependencies(connection_pool_unittest deps)
target_link_libraries(connection_pool_unittest connection_pool
                      gtest mysqlclient hiredis pthread)
install(TARGETS connection_pool_unittest DESTINATION bin)
add_test(NAME unittest_connection_pool COMMAND connection_pool_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(segment_unittest segment_unittest.cpp)
add_dependencies(segment_unittest deps)
target_link_libraries(segment_unittest segment gtest pthread)
install(TARGETS segment_unittest DESTINATION bin)
add_test(NAME unittest_segment COMMAND segment_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(transform_unittest transform_unittest.cpp)
add_dependencies(transform_unittest deps)
target_link_libraries(transform_unittest transform gtest pthread)
install(TARGETS transform_unittest DESTINATION bin)
add_test(NAME unittest_transform COMMAND transform_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(hz2py_unittest hz2py_unittest.cpp)
add_dependencies(hz2py_unittest deps)
target_link_libraries(hz2py_unittest hz2py gtest pthread)
install(TARGETS hz2py_unittest DESTINATION bin)
add_test(NAME unittest_hz2py COMMAND hz2py_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(inotify_unittest inotify_unittest.cpp)
add_dependencies(inotify_unittest deps)
target_link_libraries(inotify_unittest inotify gtest pthread)
install(TARGETS inotify_unittest DESTINATION bin)
add_test(NAME unittest_inotify COMMAND inotify_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
