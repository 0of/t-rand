cmake_minimum_required(VERSION 2.8.0)

project(t-rand)

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    fido
    PREFIX ${CMAKE_BINARY_DIR}/deps
    GIT_REPOSITORY https://github.com/0of/fido.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(fido source_dir)
include_directories(${source_dir})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
## widen template depth threshold
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=1400")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

add_subdirectory(mt)

add_custom_target(run_test COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/run_test.sh")


