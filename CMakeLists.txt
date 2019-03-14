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
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_library(
        UniSockets
        STATIC
        ${SOURCE_FILES}
)
#https://stackoverflow.com/questions/42456284/how-to-link-shared-library-dll-with-cmake-in-windows
#add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
endif()

