cmake_minimum_required(VERSION 3.13)
project(UniSockets)

set(CMAKE_CXX_STANDARD 17)

set(
        INCLUDE_DIRS

)

set(
        SOURCE_FILES
        main.cpp
        UniSocket.cpp
        UniSocket.h
        SocketWrapperWin.cpp
        SocketWrapperWin.h
)

include_directories(${INCLUDE_DIRS})
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)