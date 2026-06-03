// Copyright 2026 yx
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROBOT_BEHAVIOR__OBSTACLE_AVOIDER_HPP_
#define ROBOT_BEHAVIOR__OBSTACLE_AVOIDER_HPP_

#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

namespace robot_behavior
{

// Tunable parameters of the cruise / avoidance behavior.
struct CruiseParams
{
  double front_sector_deg;  // width of the frontal cone we watch [deg]
  double stop_distance;     // obstacle closer than this -> stop & turn [m]
  double cruise_speed;      // forward speed when the path is clear [m/s]
  double turn_speed;        // yaw speed while avoiding [rad/s]
};

// Pure decision logic: one laser frame + params -> a velocity command.
// No ROS spin, no I/O -> directly unit-testable.
geometry_msgs::msg::Twist computeCommand(
  const sensor_msgs::msg::LaserScan & scan,
  const CruiseParams & params);

// The ROS2 node: wires /scan -> computeCommand -> /cmd_vel.
class ObstacleAwareCruise : public rclcpp::Node
{
public:
  ObstacleAwareCruise();

private:
  void scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);

  CruiseParams params_;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_pub_;
};

}  // namespace robot_behavior

#endif  // ROBOT_BEHAVIOR__OBSTACLE_AVOIDER_HPP_
