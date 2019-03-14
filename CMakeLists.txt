cmake_minimum_required(VERSION 3.13)
project(UniSockets)

set(CMAKE_CXX_STANDARD 17)
set(
        SOURCE_FILES
        main.cpp
        UniSocket.h
        UniSocket.cpp
)
if(WIN32)
    list(APPEND SOURCE_FILES
            "SocketWrapperWin.cpp"
            "SocketWrapperWin.h"
    )
endif()
if(UNIX)
    list(APPEND SOURCE_FILES
            "SocketWrapperLin.cpp"
            "SocketWrapperLin.h"
    )
endif()
#add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
set(CMAKE_BUILD_TYPE Release)

add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
endif()