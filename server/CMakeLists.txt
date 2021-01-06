cmake_minimum_required(VERSION 3.2.2)

project(server)

find_package(Boost 1.73.0 REQUIRED system)

include_directories(${Boost_INCLUDE_DIRES})
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/server)

file(GLOB CPP_FILES ${CMAKE_SOURCE_DIR}/src/server/*.cpp)

add_executable(server ${CPP_FILES})

target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(server ${CMAKE_SOURCE_DIR}/build/core/libcore.a)
target_link_libraries(server ${CMAKE_SOURCE_DIR}/build/network/libnetwork.a)

set_target_properties(server PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

install (TARGETS server DESTINATION bin)
