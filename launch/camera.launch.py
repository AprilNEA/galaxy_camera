import os

from ament_index_python import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.actions import GroupAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.substitutions import TextSubstitution
from launch_ros.actions import Node
from launch_ros.actions import PushRosNamespace


def generate_launch_description():
    # args that can be set from the command line or a default will be used
    # background_r_launch_arg = DeclareLaunchArgument(
    #     "background_r", default_value=TextSubstitution(text="0")
    # )

    # include another launch file
    # launch_include = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         os.path.join(
    #             get_package_share_directory('demo_nodes_cpp'),
    #             'launch/topics/talker_listener.launch.py'))
    #

    # start a turtlesim_node in the turtlesim1 namespace
    camera_node = Node(
        package='galaxy_camera',
        namespace='camera',
        executable='camera_publisher',
        name='camera1',
        parameters=[{

        }]
    )

    # perform remap so both turtles listen to the same command topic
    # forward_turtlesim_commands_to_second_turtlesim_node = Node(
    #     package='turtlesim',
    #     executable='mimic',
    #     name='mimic',
    #     remappings=[
    #         ('/input/pose', '/turtlesim1/turtle1/pose'),
    #         ('/output/cmd_vel', '/turtlesim2/turtle1/cmd_vel'),
    #     ]
    # )

    return LaunchDescription([

    ])
