#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vkapi::vkapi" for configuration "Release"
set_property(TARGET vkapi::vkapi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vkapi::vkapi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/vkapi.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS vkapi::vkapi )
list(APPEND _IMPORT_CHECK_FILES_FOR_vkapi::vkapi "${_IMPORT_PREFIX}/lib/vkapi.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
