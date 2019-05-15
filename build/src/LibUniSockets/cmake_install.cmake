# Install script for directory: D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/LibUniSockets")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/build/lib/libLibUniSockets.dll.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xbinx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/build/bin/libLibUniSockets.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libLibUniSockets.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libLibUniSockets.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libLibUniSockets.dll")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/LibUniSockets" TYPE FILE FILES
    "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets/include/LibUniSockets/UniSocket.h"
    "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets/include/LibUniSockets/UniSocketSet.h"
    "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets/include/LibUniSockets/UnisocketException.h"
    "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets/include/LibUniSockets/SocketWrapperWin.h"
    "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/src/LibUniSockets/include/LibUniSockets/FDSetWrapperWin.h"
    )
endif()

