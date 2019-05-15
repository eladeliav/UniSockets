#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LibUniSockets::LibUniSockets" for configuration "Debug"
set_property(TARGET LibUniSockets::LibUniSockets APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(LibUniSockets::LibUniSockets PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libLibUniSockets.dll.a"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libLibUniSockets.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS LibUniSockets::LibUniSockets )
list(APPEND _IMPORT_CHECK_FILES_FOR_LibUniSockets::LibUniSockets "${_IMPORT_PREFIX}/lib/libLibUniSockets.dll.a" "${_IMPORT_PREFIX}/bin/libLibUniSockets.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
