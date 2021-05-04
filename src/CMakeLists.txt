include_directories(${PROJECT_SOURCE_DIR}/deps/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/lib)

set(THREAD_POOL_FILES thread_pool.cpp)
add_library(thread_pool SHARED ${THREAD_POOL_FILES})
install(TARGETS thread_pool DESTINATION lib)

set(CONNECTION_POOL_FILES mysql_connection.cpp redis_connection.cpp)
add_library(connection_pool SHARED ${CONNECTION_POOL_FILES})
add_dependencies(connection_pool deps)
install(TARGETS connection_pool DESTINATION lib)

set(SEGMENT_FILES segment.cpp)
add_library(segment SHARED ${SEGMENT_FILES})
install(TARGETS segment DESTINATION lib)

set(TRANSFORM_FILES transform.cpp)
add_library(transform SHARED ${TRANSFORM_FILES})
install(TARGETS transform DESTINATION lib)

set(HZ2PY_FILES hz2py.cpp)
add_library(hz2py SHARED ${HZ2PY_FILES})
install(TARGETS hz2py DESTINATION lib)

set(INOTIFY_FILES inotify.cpp)
add_library(inotify SHARED ${INOTIFY_FILES})
install(TARGETS inotify DESTINATION lib)


install(DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h")
