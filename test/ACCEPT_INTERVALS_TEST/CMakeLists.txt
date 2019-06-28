set(TEST_TARGET_NAME Accept_Intervals_Test)

if (UNIX)
    find_package(Threads)
endif ()

add_executable(${TEST_TARGET_NAME} accept_intervals_test.cpp)
target_link_libraries(${TEST_TARGET_NAME} UniSockets)
if (UNIX)
    target_link_libraries(${TEST_TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif ()


# Add a test to the project to be run by ctest.
# See https://cmake.org/cmake/help/latest/command/add_test.html
# See https://cmake.org/cmake/help/latest/manual/ctest.1.html
# COMMAND tag specifies the test command-line. If it is an executable target
# created by add_executable(), it will automatically be replaced by the location
# of the executable created at build time.
add_test(NAME ${TEST_TARGET_NAME}
        COMMAND ${TEST_TARGET_NAME}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)