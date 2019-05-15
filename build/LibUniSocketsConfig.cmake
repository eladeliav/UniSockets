set(LibUniSockets_VERSION 1.0)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was LibUniSocketsConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################



if(NOT TARGET LibUniSockets::LibUniSockets)
  include("${CMAKE_CURRENT_LIST_DIR}/LibUniSocketsTargets.cmake")
endif()

# Compatibility
get_property(LibUniSockets_LibUniSockets_INCLUDE_DIR TARGET LibUniSockets::LibUniSockets PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

set(LibUniSockets_LIBRARIES LibUniSockets::LibUniSockets)
set(LibUniSockets_INCLUDE_DIRS "${LibUniSockets_LibUniSockets_INCLUDE_DIR}")
list(REMOVE_DUPLICATES LibUniSockets_INCLUDE_DIRS)


