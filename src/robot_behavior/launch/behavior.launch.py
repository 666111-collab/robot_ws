#!/usr/bin/env python3
"""Launch the obstacle_aware_cruise node with parameters from a config yaml."""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('robot_behavior')
    default_params = os.path.join(pkg_dir, 'config', 'obstacle_avoider.yaml')

    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    params_file = LaunchConfiguration('params_file', default=default_params)

    cruise_node = Node(
        package='robot_behavior',
        executable='obstacle_aware_cruise',
        name='obstacle_aware_cruise',
        output='screen',
        parameters=[params_file, {'use_sim_time': use_sim_time}],
    )

    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='true'),
        DeclareLaunchArgument('params_file', default_value=default_params),
        cruise_node,
    ])
