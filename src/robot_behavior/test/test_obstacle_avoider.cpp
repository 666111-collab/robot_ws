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

#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "robot_behavior/obstacle_avoider.hpp"

using robot_behavior::computeCommand;
using robot_behavior::CruiseParams;

namespace
{
// Fixed parameter set shared by all cases (front cone = +-30 deg).
CruiseParams makeParams()
{
  CruiseParams p;
  p.front_sector_deg = 60.0;
  p.stop_distance = 0.5;
  p.cruise_speed = 0.15;
  p.turn_speed = 0.6;
  return p;
}

// A 360-beam TB3-like scan with every beam initialized to `fill` meters.
sensor_msgs::msg::LaserScan makeScan(double fill)
{
  sensor_msgs::msg::LaserScan scan;
  scan.angle_min = 0.0;
  scan.angle_max = 2.0 * M_PI;
  scan.angle_increment = 2.0 * M_PI / 360.0;
  scan.range_min = 0.12;
  scan.range_max = 3.5;
  scan.ranges.assign(360, static_cast<float>(fill));
  return scan;
}

// Index of the beam pointing at `deg` degrees (0 = straight ahead).
size_t beamAt(double deg)
{
  double rad = deg * M_PI / 180.0;
  if (rad < 0.0) {
    rad += 2.0 * M_PI;
  }
  return static_cast<size_t>(std::round(rad / (2.0 * M_PI / 360.0))) % 360;
}
}  // namespace

TEST(ObstacleAvoider, ClearPathCruisesForward)
{
  const auto cmd = computeCommand(makeScan(3.0), makeParams());
  EXPECT_DOUBLE_EQ(cmd.linear.x, 0.15);
  EXPECT_DOUBLE_EQ(cmd.angular.z, 0.0);
}

TEST(ObstacleAvoider, ObstacleAheadStopsAndTurns)
{
  auto scan = makeScan(3.0);
  scan.ranges[beamAt(0.0)] = 0.3f;  // wall 0.3 m dead ahead (< 0.5 stop_distance)
  const auto cmd = computeCommand(scan, makeParams());
  EXPECT_DOUBLE_EQ(cmd.linear.x, 0.0);
  EXPECT_DOUBLE_EQ(cmd.angular.z, 0.6);
}

TEST(ObstacleAvoider, ObstacleToTheSideIsIgnored)
{
  auto scan = makeScan(3.0);
  scan.ranges[beamAt(90.0)] = 0.2f;  // obstacle at 90 deg, outside the front cone
  const auto cmd = computeCommand(scan, makeParams());
  EXPECT_DOUBLE_EQ(cmd.linear.x, 0.15);  // should still cruise straight
  EXPECT_DOUBLE_EQ(cmd.angular.z, 0.0);
}

TEST(ObstacleAvoider, InvalidReadingsAreIgnored)
{
  auto scan = makeScan(3.0);
  scan.ranges[beamAt(0.0)] = std::numeric_limits<float>::infinity();
  scan.ranges[beamAt(5.0)] = std::numeric_limits<float>::quiet_NaN();
  const auto cmd = computeCommand(scan, makeParams());
  EXPECT_DOUBLE_EQ(cmd.linear.x, 0.15);  // inf/nan dropped -> path counts as clear
  EXPECT_DOUBLE_EQ(cmd.angular.z, 0.0);
}
