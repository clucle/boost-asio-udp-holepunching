cmake_minimum_required(VERSION 3.2.2)

project(core)

find_package(Boost 1.73.0 REQUIRED system)
include_directories(${Boost_INCLUDE_DIRES})

file(GLOB CPP_FILES *.cpp)

add_library(core STATIC
    ${CPP_FILES})

target_link_libraries(core ${Boost_LIBRARIES})

set_target_properties(core PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

set_target_properties(core PROPERTIES PUBLIC_HEADER include)

target_include_directories(core PRIVATE .)