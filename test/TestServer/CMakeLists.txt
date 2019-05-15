set(TEST_TARGET_NAME test_TestServer)

set(${TEST_TARGET_NAME}_SRC
        server.cpp
)

set(LibUniSockets_DIR "D:\\Programming\\VisualStudioProjects\\CPP\\VSCodeVersions\\UniSockets\\build")

find_package(LibUniSockets)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})

target_link_libraries(${TEST_TARGET_NAME} LibUniSockets::LibUniSockets)
# Add a test to the project to be run by ctest.
# See https://cmake.org/cmake/help/latest/command/add_test.html
# See https://cmake.org/cmake/help/latest/manual/ctest.1.html
# COMMAND tag specifies the test command-line. If it is an executable target
# created by add_executable(), it will automatically be replaced by the location
# of the executable created at build time.
add_test(NAME ${TEST_TARGET_NAME}
         COMMAND ${TEST_TARGET_NAME}
         WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)