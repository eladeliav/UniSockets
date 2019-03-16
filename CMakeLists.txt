cmake_minimum_required(VERSION 3.13)
project(UniSockets)

set(CMAKE_CXX_STANDARD 17)
set(
        JUST_HEADERS
        UniSocket.h
        UniSocketSet.h
)
set(
        SOURCE_FILES
        UniSocket.h
        UniSocket.cpp
        UniSocketSet.h
        UniSocketSet.cpp
        main.cpp
)
if (WIN32)
    list(APPEND SOURCE_FILES
            "SocketWrapperWin.cpp"
            "SocketWrapperWin.h"
            )
    list(APPEND JUST_HEADERS
            "SocketWrapperWin.h")
    set(OUTPUT_INCLUDE_DIR "D:/Programming/VisualStudioProjects/various includes/UniSockets/include/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "D:/Programming/VisualStudioProjects/various includes/UniSockets/lib/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "D:/Programming/VisualStudioProjects/various includes/UniSockets/lib/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "D:/Programming/VisualStudioProjects/various includes/UniSockets/lib/")
endif ()
if (UNIX)
    list(APPEND SOURCE_FILES
            "SocketWrapperLin.cpp"
            "SocketWrapperLin.h"
            )
    list(APPEND JUST_HEADERS
            "SocketWrapperLin.h")
    set(OUTPUT_INCLUDE_DIR "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/include/")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/lib/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/lib/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/eladeliav/Documents/Programming/C++/Various Includes/UniSockets/lib/")
endif ()

message("include output: ${OUTPUT_INCLUDE_DIR}")
message("lib output: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
file(COPY ${JUST_HEADERS}
        DESTINATION ${OUTPUT_INCLUDE_DIR})

add_library(
        ${PROJECT_NAME}
        SHARED
        ${SOURCE_FILES}
)

#add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif ()

