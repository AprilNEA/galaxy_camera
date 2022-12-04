cmake_minimum_required(VERSION 3.8)
project(galaxy_camera)

# Default to C++17
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

find_package(ament_cmake REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS highgui imgcodecs imgproc videoio)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)
include_directories(include/libgxiapi)

# add the resized image message
set(msg_files
        "msg/ResizedImage.msg"
        )
rosidl_generate_interfaces(${PROJECT_NAME}
        ${msg_files}
        DEPENDENCIES sensor_msgs
        )

rosidl_get_typesupport_target(cpp_typesupport_target "${PROJECT_NAME}" "rosidl_typesupport_cpp")

# add the publisher example
#add_executable(my_publisher src/my_publisher.cpp)
#target_link_libraries(my_publisher PRIVATE gxiapi
#  cv_bridge::cv_bridge
#  image_transport::image_transport
#  opencv_imgcodecs
#  rclcpp::rclcpp
#)

add_executable(camera_parameters src/camera_parameters.cpp)
ament_target_dependencies(camera_parameters
        rclcpp)

# add the subscriber example
add_executable(camera_subscriber
        src/camera_subscriber.cpp)
target_link_libraries(camera_subscriber
        cv_bridge::cv_bridge
        image_transport::image_transport
        opencv_highgui
        rclcpp::rclcpp
        gxiapi
        )

# add the publisher from video example
add_executable(camera_publisher
        src/camera_publisher.cpp src/galaxy_camera.cpp)
target_link_libraries(camera_publisher
        cv_bridge::cv_bridge
        image_transport::image_transport
        opencv_highgui
        opencv_videoio
        rclcpp::rclcpp
        gxiapi
        "${sensor_msgs_TARGETS}"
        "${std_msgs_TARGETS}"
        )

# Install executables
install(
        TARGETS camera_parameters camera_publisher camera_subscriber
        RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Install include directories
install(
        DIRECTORY include/
        DESTINATION include/${PROJECT_NAME}
)
ament_export_include_directories(include)
ament_export_dependencies(cv_bridge image_transport pluginlib rosidl_default_runtime rclcpp sensor_msgs std_msgs
        gxiapi)

ament_package()
