// generated from
// rosidl_typesupport_introspection_c/resource/rosidl_typesupport_introspection_c__visibility_control.h.in
// generated code does not contain a copyright notice

#ifndef IMAGE_TRANSPORT_TUTORIALS__MSG__ROSIDL_TYPESUPPORT_INTROSPECTION_C__VISIBILITY_CONTROL_H_
#define IMAGE_TRANSPORT_TUTORIALS__MSG__ROSIDL_TYPESUPPORT_INTROSPECTION_C__VISIBILITY_CONTROL_H_

#ifdef __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_image_transport_tutorials __attribute__ ((dllexport))
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_IMPORT_image_transport_tutorials __attribute__ ((dllimport))
  #else
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_image_transport_tutorials __declspec(dllexport)
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_IMPORT_image_transport_tutorials __declspec(dllimport)
  #endif
  #ifdef ROSIDL_TYPESUPPORT_INTROSPECTION_C_BUILDING_DLL_image_transport_tutorials
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_PUBLIC_image_transport_tutorials ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_image_transport_tutorials
  #else
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_PUBLIC_image_transport_tutorials ROSIDL_TYPESUPPORT_INTROSPECTION_C_IMPORT_image_transport_tutorials
  #endif
#else
  #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_image_transport_tutorials __attribute__ ((visibility("default")))
  #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_IMPORT_image_transport_tutorials
  #if __GNUC__ >= 4
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_PUBLIC_image_transport_tutorials __attribute__ ((visibility("default")))
  #else
    #define ROSIDL_TYPESUPPORT_INTROSPECTION_C_PUBLIC_image_transport_tutorials
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif  // IMAGE_TRANSPORT_TUTORIALS__MSG__ROSIDL_TYPESUPPORT_INTROSPECTION_C__VISIBILITY_CONTROL_H_
