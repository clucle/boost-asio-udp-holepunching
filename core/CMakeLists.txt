cmake_minimum_required(VERSION 3.2.2)

project(core)

find_package(Boost 1.73.0 REQUIRED system)

include_directories(${Boost_INCLUDE_DIRES})
include_directories(${CMAKE_SOURCE_DIR}/includes/core)

file(GLOB CPP_FILES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)

add_library(core STATIC ${CPP_FILES})

target_link_libraries(core ${Boost_LIBRARIES})

set_target_properties(core PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

set_target_properties(core PROPERTIES PUBLIC_HEADER includes/core)

target_include_directories(core PRIVATE .)