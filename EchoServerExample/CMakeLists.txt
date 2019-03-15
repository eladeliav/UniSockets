cmake_minimum_required(VERSION 3.13)
project(UniSockClient LANGUAGES CXX)

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

#move exe and dll to be next to each other
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#find dll
if(APPLE)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
    find_library(
            UNISOCK_LIB_APPLE
            NAMES libUniSockets.dylib
            PATH_SUFFIXES .dylib
            PATHS ${MY_LIB_PATH}
    )
    file(COPY ${UNISOCK_LIB_APPLE}
            DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    set(UNISOCK_LIB_STATIC ${UNISOCK_LIB_APPLE})
endif()
if(UNIX AND NOT APPLE)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
    find_library(
            UNISOCK_LIB_UNIX
            NAMES libUniSockets.so
            PATH_SUFFIXES .so
            PATHS ${MY_LIB_PATH}
    )
    file(COPY ${UNISOCK_LIB_UNIX}
            DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    set(UNISOCK_LIB_STATIC ${UNISOCK_LIB_UNIX})
endif()
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
    find_library(
            UNISOCK_LIB_WIN
            NAMES libUniSockets.dll
            PATH_SUFFIXES .dll
            PATHS ${MY_LIB_PATH}
    )
    file(COPY ${UNISOCK_LIB_WIN}
            DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    set(UNISOCK_LIB_STATIC ${UNISOCK_LIB_WIN})
endif()

message("lib suffixes ${CMAKE_FIND_LIBRARY_SUFFIXES}")

#add executable and link dll to executable
add_executable(${PROJECT_NAME} main.cpp)
message("linking lib: ${UNISOCK_LIB_STATIC}")
target_link_libraries(${PROJECT_NAME} ${UNISOCK_LIB_STATIC})