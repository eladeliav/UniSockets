cmake_minimum_required(VERSION 3.13)
project(UniSocketsTesting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    set(
            INCLUDE_DIRS
            "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/include/"
    )
    set(
            MY_LIB_PATH
            "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/lib/"
    )
endif ()

if (WIN32)
    set(
            INCLUDE_DIRS
            "D:/Programming/VisualStudioProjects/various includes/UniSockets/include/"
    )
    set(
            MY_LIB_PATH
            "D:/Programming/VisualStudioProjects/various includes/UniSockets/lib/"
    )
endif ()

#include header files

message("include dirs: ${INCLUDE_DIRS}")
message("lib path: ${MY_LIB_PATH}")

include_directories(${INCLUDE_DIRS})

#find dll

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
message("lib suffixes ${CMAKE_FIND_LIBRARY_SUFFIXES}")
find_library(
        UNISOCK_LIB_STATIC
        NAMES libUniSockets.a
        PATH_SUFFIXES .a
        PATHS ${MY_LIB_PATH}
)
message("found lib ${UNISOCK_LIB_STATIC}")
#move exe and dll to be next to each other
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(COPY ${UNISOCK_LIB_STATIC}
        DESTINATION ${EXECUTABLE_OUTPUT_PATH})

#add executable and link dll to executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${UNISOCK_LIB_STATIC})