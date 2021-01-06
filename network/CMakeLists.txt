cmake_minimum_required(VERSION 3.2.2)

project(network)

find_package(Boost 1.73.0 REQUIRED system)

include_directories(${Boost_INCLUDE_DIRES})
include_directories(${CMAKE_SOURCE_DIR}/includes/network)

file(GLOB CPP_FILES ${CMAKE_SOURCE_DIR}/src/network/*.cpp)

add_library(network STATIC ${CPP_FILES})

target_link_libraries(network ${Boost_LIBRARIES})

set_target_properties(network PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

set_target_properties(network PROPERTIES PUBLIC_HEADER includes/network)

target_include_directories(network PRIVATE .)