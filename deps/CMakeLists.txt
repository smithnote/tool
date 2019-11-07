include(ExternalProject)
add_custom_target(deps)

set(DEPS_INSTALL_DIR "${PROJECT_SOURCE_DIR}/deps")
set(DEPS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps")

ExternalProject_Add(
    gflags_project
    GIT_REPOSITORY https://github.com/gflags/gflags
    PREFIX ${DEPS_SOURCE_DIR}/gflags
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
               -DBUILD_SHARED_LIBS=OFF
    LOG_DOWNLOAD 1
    LOG_INSTALL 1
)

ExternalProject_Add(
    googletest_project
    GIT_REPOSITORY https://github.com/google/googletest
    PREFIX ${DEPS_SOURCE_DIR}/gtest
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
               -DBUILD_SHARED_LIBS=OFF
    LOG_DOWNLOAD 1
    LOG_INSTALL 1
)

ExternalProject_Add(
    glog_project
    GIT_REPOSITORY https://github.com/google/glog
    PREFIX ${DEPS_SOURCE_DIR}/glog
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
               -DBUILD_SHARED_LIBS=OFF
    LOG_DOWNLOAD 1
    LOG_INSTALL 1
)

ExternalProject_Add(
    hiredis_project
    GIT_REPOSITORY https://github.com/redis/hiredis
    PREFIX ${DEPS_SOURCE_DIR}/hiredis
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
    LOG_DOWNLOAD 1
    LOG_INSTALL 1
)

add_dependencies(deps
    googletest_project
    gflags_project
    hiredis_project
)
install(DIRECTORY ${DEPS_INSTALL_DIR}/lib DESTINATION .)
