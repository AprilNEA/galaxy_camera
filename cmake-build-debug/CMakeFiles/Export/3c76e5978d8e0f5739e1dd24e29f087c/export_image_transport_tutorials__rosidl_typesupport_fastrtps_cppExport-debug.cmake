#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "galaxy_camera::image_transport_tutorials__rosidl_typesupport_fastrtps_cpp" for configuration "Debug"
set_property(TARGET image_transport_tutorials::image_transport_tutorials__rosidl_typesupport_fastrtps_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(image_transport_tutorials::image_transport_tutorials__rosidl_typesupport_fastrtps_cpp PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libimage_transport_tutorials__rosidl_typesupport_fastrtps_cpp.so"
  IMPORTED_SONAME_DEBUG "libimage_transport_tutorials__rosidl_typesupport_fastrtps_cpp.so"
  )

list(APPEND _cmake_import_check_targets image_transport_tutorials::image_transport_tutorials__rosidl_typesupport_fastrtps_cpp )
list(APPEND _cmake_import_check_files_for_image_transport_tutorials::image_transport_tutorials__rosidl_typesupport_fastrtps_cpp "${_IMPORT_PREFIX}/lib/libimage_transport_tutorials__rosidl_typesupport_fastrtps_cpp.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
