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

#include <functional>

#include "robot_behavior/obstacle_avoider.hpp"

namespace robot_behavior
{

ObstacleAwareCruise::ObstacleAwareCruise()
: Node("obstacle_aware_cruise")
{
  // --- Declare tunable parameters (with safe defaults) and read them in. ---
  params_.front_sector_deg = declare_parameter<double>("front_sector_deg", 60.0);
  params_.stop_distance = declare_parameter<double>("stop_distance", 0.5);
  params_.cruise_speed = declare_parameter<double>("cruise_speed", 0.15);
  params_.turn_speed = declare_parameter<double>("turn_speed", 0.6);

  // --- Publisher: /cmd_vel as TwistStamped (TB3 Harmonic bridge expects stamped). ---
  cmd_pub_ = create_publisher<geometry_msgs::msg::TwistStamped>("cmd_vel", 10);

  // --- Subscriber: /scan with sensor-data QoS (best-effort, matches LiDAR). ---
  scan_sub_ = create_subscription<sensor_msgs::msg::LaserScan>(
    "scan", rclcpp::SensorDataQoS(),
    std::bind(&ObstacleAwareCruise::scanCallback, this, std::placeholders::_1));

  RCLCPP_INFO(
    get_logger(),
    "obstacle_aware_cruise started: front=%.0f deg, stop=%.2f m, cruise=%.2f m/s, turn=%.2f rad/s",
    params_.front_sector_deg, params_.stop_distance,
    params_.cruise_speed, params_.turn_speed);
}

void ObstacleAwareCruise::scanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
  // Run the pure decision logic on this laser frame.
  const geometry_msgs::msg::Twist twist = computeCommand(*msg, params_);

  // Wrap into TwistStamped: the TB3 Harmonic bridge subscribes to a stamped msg.
  geometry_msgs::msg::TwistStamped stamped;
  stamped.header.stamp = now();
  stamped.header.frame_id = "base_link";
  stamped.twist = twist;

  cmd_pub_->publish(stamped);
}

}  // namespace robot_behavior

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<robot_behavior::ObstacleAwareCruise>());
  rclcpp::shutdown();
  return 0;
}
