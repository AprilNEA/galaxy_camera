cmake_minimum_required(VERSION 3.22)
project(DH_Camera)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-std=c++17)
#

include_directories(${PROJECT_SOURCE_DIR}/include)

#link_directories(${PROJECT_SOURCE_DIR}/lib/darwin)
#add_executable(DH_Camera main.cpp)
#target_link_libraries(DH_Camera -lgxiapi)

find_package(OpenCV REQUIRED)


include_directories(
        /usr/local/include/
        #        ${glog_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

link_directories(${PROJECT_SOURCE_DIR}/lib/darwin)

add_executable(
        DH_camera_read
        src/CamWrapper.cpp
        include/CamWrapper.h
        include/CamWrapperDH.h
        include/DxImageProc.h
        include/GxIAPI.h
        src/DH_CAM.cpp include/DH_CAM.h)

target_link_libraries(
        DH_camera_read
        ${OpenCV_LIBRARIES}
        -lglog
        -lgxiapi
)


