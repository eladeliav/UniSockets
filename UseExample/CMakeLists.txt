cmake_minimum_required(VERSION 3.13)
project(UniSocketsTesting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#include header files
set(
        INCLUDE_DIRS
        "D:/Programming/VisualStudioProjects/various includes/UniSockets/include/"
)
include_directories(${INCLUDE_DIRS})

#find dll
find_library(
        UNISOCK_LIB
        NAMES libUniSockets.dll
        PATHS "D:/Programming/VisualStudioProjects/various includes/UniSockets/lib/"
)
#move exe and dll to be next to each other
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(COPY ${UNISOCK_LIB}
        DESTINATION ${EXECUTABLE_OUTPUT_PATH})

#add executable and link dll to executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${UNISOCK_LIB})