#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "VKAPI::VKAPI" for configuration "Release"
set_property(TARGET VKAPI::VKAPI APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(VKAPI::VKAPI PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/VKAPI.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS VKAPI::VKAPI )
list(APPEND _IMPORT_CHECK_FILES_FOR_VKAPI::VKAPI "${_IMPORT_PREFIX}/lib/VKAPI.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
