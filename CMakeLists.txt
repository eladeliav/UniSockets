cmake_minimum_required(VERSION 3.13)
project(UniSockets)

set(CMAKE_CXX_STANDARD 17)

set(
        INCLUDE_DIRS
        "D:\\Programming\\VisualStudioProjects\\various includes"
        "D:\\Programming\\VisualStudioProjects\\boost_1_69_0"
)

set(
        SOURCE_FILES
        main.cpp
        main.h
)

include_directories(${INCLUDE_DIRS})
add_executable(UniSockets ${SOURCE_FILES})