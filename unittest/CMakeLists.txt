include_directories(/usr/local/include)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(/usr/local/lib)


add_executable(thread_pool_unittest thread_pool_unittest.cpp)
target_link_libraries(thread_pool_unittest thread_pool gtest pthread)
install(TARGETS thread_pool_unittest DESTINATION bin)
add_test(NAME unittest_thread_pool COMMAND thread_pool_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(connection_pool_unittest connection_pool_unittest.cpp)
target_link_libraries(connection_pool_unittest connection_pool
                      gtest mysqlclient hiredis pthread)
install(TARGETS connection_pool_unittest DESTINATION bin)
add_test(NAME unittest_connection_pool COMMAND connection_pool_unittest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
