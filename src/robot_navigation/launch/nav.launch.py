#!/usr/bin/env python3
"""Nav2 bringup for TurtleBot3 burger on the M3 map.

Usage:
    ros2 launch robot_navigation nav.launch.py \
        map:=$HOME/robot_ws/maps/turtlebot3_world.yaml
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    pkg_share = get_package_share_directory('robot_navigation')
    nav2_bringup_share = get_package_share_directory('nav2_bringup')

    default_map = os.path.join(
        os.path.expanduser('~'), 'robot_ws', 'maps', 'turtlebot3_world.yaml'
    )
    default_params = os.path.join(pkg_share, 'config', 'nav2_params.yaml')
    default_rviz = os.path.join(pkg_share, 'rviz', 'nav.rviz')

    use_sim_time = LaunchConfiguration('use_sim_time')
    map_yaml = LaunchConfiguration('map')
    params_file = LaunchConfiguration('params_file')
    use_rviz = LaunchConfiguration('use_rviz')
    autostart = LaunchConfiguration('autostart')

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time', default_value='true',
        description='Use Gazebo simulation clock.',
    )
    declare_map = DeclareLaunchArgument(
        'map', default_value=default_map,
        description='Full path to the map yaml file (from M3).',
    )
    declare_params = DeclareLaunchArgument(
        'params_file', default_value=default_params,
        description='Full path to Nav2 parameters yaml.',
    )
    declare_use_rviz = DeclareLaunchArgument(
        'use_rviz', default_value='true',
        description='Launch RViz2 with the Nav2 view.',
    )
    declare_autostart = DeclareLaunchArgument(
        'autostart', default_value='true',
        description='Auto-activate Nav2 lifecycle nodes.',
    )

    nav2_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(nav2_bringup_share, 'launch', 'bringup_launch.py')
        ),
        launch_arguments={
            'map': map_yaml,
            'use_sim_time': use_sim_time,
            'params_file': params_file,
            'autostart': autostart,
            'use_composition': 'True',
        }.items(),
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', default_rviz],
        parameters=[{'use_sim_time': use_sim_time}],
        condition=IfCondition(use_rviz),
        output='screen',
    )

    return LaunchDescription([
        declare_use_sim_time,
        declare_map,
        declare_params,
        declare_use_rviz,
        declare_autostart,
        nav2_bringup,
        rviz,
    ])
