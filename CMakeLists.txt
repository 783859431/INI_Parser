cmake_minimum_required(VERSION 3.5.0)
project(iniParser)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (sources
  ini.h
  inilib.cpp
)
add_library(iniStatic STATIC ${sources})
add_library(iniDynamic SHARED ${sources})

add_executable(testStatic example/main.c)
target_link_libraries(testStatic iniStatic)

add_executable(testDynamic example/main.c)
target_link_libraries(testDynamic PRIVATE iniDynamic)
