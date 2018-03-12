cmake_minimum_required(VERSION 2.6.)
set(CMAKE_CXX_STANDARD 11)
add_definitions(-D_DEBUG)

include_directories(source/cardanus)
add_subdirectory(source)
add_subdirectory(test)