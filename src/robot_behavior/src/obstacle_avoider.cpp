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

#include "robot_behavior/obstacle_avoider.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace robot_behavior
{

namespace
{
// Wrap any angle into the [-pi, pi] range so "forward" is always near 0.
double normalizeAngle(double angle)
{
  while (angle > M_PI) {
    angle -= 2.0 * M_PI;
  }
  while (angle < -M_PI) {
    angle += 2.0 * M_PI;
  }
  return angle;
}
}  // namespace

geometry_msgs::msg::Twist computeCommand(
  const sensor_msgs::msg::LaserScan & scan,
  const CruiseParams & params)
{
  // Half-width of the frontal cone we care about, in radians.
  const double half_sector = (params.front_sector_deg * M_PI / 180.0) / 2.0;

  // Scan the frontal cone for the closest *valid* obstacle.
  double min_front = std::numeric_limits<double>::infinity();
  for (size_t i = 0; i < scan.ranges.size(); ++i) {
    const double angle = normalizeAngle(scan.angle_min + i * scan.angle_increment);
    if (std::abs(angle) > half_sector) {
      continue;  // beam points outside the frontal cone
    }
    const double range = scan.ranges[i];
    if (!std::isfinite(range) || range < scan.range_min || range > scan.range_max) {
      continue;  // drop inf / nan / out-of-spec readings
    }
    min_front = std::min(min_front, range);
  }

  geometry_msgs::msg::Twist cmd;
  if (min_front < params.stop_distance) {
    // Obstacle ahead: stop forward motion and turn in place to look for a gap.
    cmd.linear.x = 0.0;
    cmd.angular.z = params.turn_speed;
  } else {
    // Path clear: cruise straight ahead.
    cmd.linear.x = params.cruise_speed;
    cmd.angular.z = 0.0;
  }
  return cmd;
}

}  // namespace robot_behavior
