cmake_minimum_required(VERSION 3.1)

project(P1_Energy-Monitoring C)

set(CMAKE_C_STANDARD 99)

# we start the inculsion of an external project here
include(ExternalProject) # tells CMake that we use an external project
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external) # tells cmake where to install it
ExternalProject_add(jsonc-ext # we add an external dependency and call it jsonc-ext
    GIT_REPOSITORY https://github.com/json-c/json-c # where to get the library from
    GIT_TAG 9021cdcdd01fc9dbcbe1f06391848c2ac915212f # which specific git hash to use (i.e. version), it is best practice to use a stable version, NOT their main branch (which I do here)
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_BUILD_TYPE=Release -DCMAKE_SKIP_RPATH=FALSE -DCMAKE_MACOSX_RPATH=1 -DCMAKE_INSTALL_RPATH=${EXTERNAL_INSTALL_LOCATION}/lib # build flags
) 

# tell CMake where to find the external libraries
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib64)

# tell CMake where to find the external headers
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

add_executable(P1_Energy-Monitoring main.c)

add_dependencies(P1_Energy-Monitoring jsonc-ext) # tells cmake to wait with building jsonctest untill  json-c has been build
target_link_libraries(P1_Energy-Monitoring PRIVATE json-c) # link with the newly build library

# this is for the curl library
set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(P1_Energy-Monitoring PRIVATE ${CURL_LIBRARIES})