cmake_minimum_required(VERSION 3.22)
project(DH_Camera)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-std=c++17)


# 本项目的头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 根据平台选择加载的动态连接库位置
#cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
#message(STATUS "${PRETTY_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    link_directories(${PROJECT_SOURCE_DIR}/lib/linux_x86)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "current platform: Mac OS X")
    link_directories(${PROJECT_SOURCE_DIR}/lib/darwin)
    link_directories(${PROJECT_SOURCE_DIR}/lib/darwin)
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

find_package(OpenCV REQUIRED)

#include_directories(
#        ${glog_INCLUDE_DIR}
#        ${OpenCV_INCLUDE_DIRS}
#)

add_executable(
        DH_camera_read
        src/CamWrapper.cpp
        include/CamWrapper.h
        include/DxImageProc.h
        include/GxIAPI.h
        src/DH_Camera.cpp include/DH_Camera.h)

#target_link_libraries(
#        DH_camera_read
#        ${OpenCV_LIBRARIES}
#        -lglog
#        -lgxiapi
#)

add_executable(main main.cpp)
target_link_libraries(main -lgxiapi)