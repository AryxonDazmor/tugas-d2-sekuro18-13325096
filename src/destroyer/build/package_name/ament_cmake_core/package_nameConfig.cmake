# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_package_name_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED package_name_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(package_name_FOUND FALSE)
  elseif(NOT package_name_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(package_name_FOUND FALSE)
  endif()
  return()
endif()
set(_package_name_CONFIG_INCLUDED TRUE)

# output package information
if(NOT package_name_FIND_QUIETLY)
  message(STATUS "Found package_name: 0.0.0 (${package_name_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'package_name' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${package_name_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(package_name_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${package_name_DIR}/${_extra}")
endforeach()
