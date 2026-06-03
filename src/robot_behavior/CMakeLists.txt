cmake_minimum_required(VERSION 3.8)
project(robot_behavior)

# --- Modern C++ standard ---
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Find dependencies ---
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

# --- Library: avoidance logic (header + impl), shared by node and tests ---
add_library(obstacle_avoider_lib
  src/obstacle_avoider.cpp
)
target_include_directories(obstacle_avoider_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(obstacle_avoider_lib
  rclcpp
  sensor_msgs
  geometry_msgs
)

# --- Executable: the ROS2 node ---
add_executable(obstacle_aware_cruise
  src/obstacle_avoider_node.cpp
)
target_link_libraries(obstacle_aware_cruise obstacle_avoider_lib)
ament_target_dependencies(obstacle_aware_cruise
  rclcpp
  sensor_msgs
  geometry_msgs
)

# --- Install ---
install(TARGETS
  obstacle_aware_cruise
  obstacle_avoider_lib
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)
install(DIRECTORY include/
  DESTINATION include
)

# --- Tests ---
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_obstacle_avoider
    test/test_obstacle_avoider.cpp
  )
  target_link_libraries(test_obstacle_avoider obstacle_avoider_lib)
  ament_target_dependencies(test_obstacle_avoider
    rclcpp
    sensor_msgs
    geometry_msgs
  )
endif()

ament_package()
