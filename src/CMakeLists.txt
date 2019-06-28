set(LIBRARY_TARGET_NAME ${PROJECT_NAME})
# List of CPP (source) library files.

set(${LIBRARY_TARGET_NAME}_SRC
        "src/UniSocket.cpp"
        "src/UniSocketException.cpp"
        "src/UniServerSocket.cpp"
        )

# List of HPP (header) library files.
set(${LIBRARY_TARGET_NAME}_HDR
        "include/UniSockets/Core.hpp"
        "include/UniSockets/UniSocket.hpp"
        "include/UniSockets/UniSocketException.hpp"
        "include/UniSockets/UniServerSocket.hpp"
        )

#if (WIN32)
#    set(CMAKE_SHARED_LIBRARY_PREFIX "")
#    set(CMAKE_STATIC_LIBRARY_SUFFIX ".dll.a")
#endif ()

add_library(${LIBRARY_TARGET_NAME} STATIC ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

# creating alias for within the project
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

# Let's set compiler-specific flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # G++
    target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE -Wall -Wextra -Werror)
    #set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC
    target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE /EHsc /MTd /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    target_compile_definitions(roukavici PRIVATE WIN_EXPORT)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif ()

# winsock dependency
if (WIN32)
    target_link_libraries(${LIBRARY_TARGET_NAME} ws2_32)
endif ()

# Set two minimum target properties for the library.
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}"
        )

# Specify include directories for both compilation and installation process.
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
        )

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)


message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")