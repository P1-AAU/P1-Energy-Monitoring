cmake_minimum_required(VERSION 3.0.0)
project(P1-Energy-Monitoring VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(P1-Energy-Monitoring main.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(P1-Energy-Monitoring ${CURL_LIBRARIES})

find_package(json-c CONFIG)
target_link_libraries(P1-Energy-Monitoring json-c::json-c)