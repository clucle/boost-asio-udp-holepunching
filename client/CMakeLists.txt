cmake_minimum_required(VERSION 3.2.2)

project(client)

find_package(Boost 1.73.0 REQUIRED system)

include_directories(${Boost_INCLUDE_DIRES})
include_directories(${CMAKE_SOURCE_DIR}/core)

file(GLOB CPP_FILES *.cpp)

add_executable(client ${CPP_FILES})

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(client ${CMAKE_SOURCE_DIR}/build/core/libcore.a)

install (TARGETS client DESTINATION bin)
