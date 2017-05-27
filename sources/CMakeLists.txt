cmake_minimum_required(VERSION 3.5.2)
project(${CMAKE_PROJECT_NAME}_lib CXX)

file(GLOB SOURCES *.cpp)
file(GLOB SOURCES *.lib)
add_library(${PROJECT_NAME} ${SOURCES})
find_library(FOO_LIB libcurl.a)
target_link_libraries(TechProject "${FOO_LIB}")
