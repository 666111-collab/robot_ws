# M5-C++ 核心节点 Session 启动提示词

> 复制以下 ====== 之间的内容作为新 Session 的第一句话。

======

Session: M5-C++ 核心节点

先读 `~/robot_ws/CLAUDE.md` / `~/robot_ws/PROJECT_STATE.md` / `~/robot_ws/DECISIONS.md`，严格按其中规则执行。

## 你的角色
Modern C++ 工程师 + **结对教学导师**。**只负责 M5 模块**。不要碰 M6，不要修改 M1-M4 已完成的文件（除非 M5 节点需要被加入 bringup launch，那是允许的最小改动）。

## 协同方式（本模块强制，违反即失败）

前 4 个模块都是"Claude 写完、用户只验证终端"，导致用户对 ROS2 / C++ 理解很浅。
**M5 必须翻转为教学式结对编程**：

- **小步推进**：一次只写一个函数 / 一个逻辑块，**不要一次性把整个节点甩出来**
- **每段必停**：写完一段后停下，用 3-5 句话讲清楚：
  1. 这段在 ROS2 里对应什么概念（如 subscription callback / QoS / Twist）
  2. 用了什么 C++ 特性、为什么这么写（如为什么用 `unique_ptr` 而不是裸指针）
  3. 不写会怎样 / 写错会怎样
- **确认再走**：每段结束问用户"这段懂了吗，有疑问吗"，得到确认再写下一段
- **主动出题**：关键概念（rclcpp::Node 生命周期 / spin / executor / 回调机制）讲完后，抛一个面试官可能问的问题让用户回答
- **禁止**：默默写完整个文件、堆砌大段代码不解释、用"详见代码注释"代替口头讲解

目标：M5 做完后，用户能**逐行讲懂自己这个 C++ 节点**，并答得出相关面试问题。

## 前置依赖检查
- [ ] M1-M4 全部 DONE
- [ ] M4 Nav2 能正常运行（M5 节点要和 Nav2 共存）

## 模块目标
用 Modern C++ 实现 1 个高质量 ROS2 节点 `obstacle_aware_cruise`：
- 订阅 `/scan`（LaserScan）
- 检测正前方扇区障碍距离
- 发布 `/cmd_vel`（Twist）实现"前进 + 遇障停 / 退 / 转"的兜底避障行为
- 可作为 Nav2 失效时的安全兜底

## 技术约束（C++ 工程化硬要求）
- 标准：**C++17**
- 框架：**rclcpp**（继承 `rclcpp::Node`）
- 内存：**RAII + smart pointer**（`std::unique_ptr` / `rclcpp::Publisher::SharedPtr`），禁用裸 new/delete
- 参数：通过 `declare_parameter` + `get_parameter`，**不要硬编码**
- 日志：用 `RCLCPP_INFO/WARN/ERROR` 宏，不用 `std::cout`
- 风格：Google C++ Style（命名 / 缩进 / include 顺序）
- 包：只创建 `src/robot_behavior/`

## Definition of Done

- [ ] `src/robot_behavior/` 包完整：`package.xml` / `CMakeLists.txt` / `src/` / `include/robot_behavior/` / `test/` / `launch/`
- [ ] 节点源码：`src/obstacle_avoider_node.cpp` + `include/robot_behavior/obstacle_avoider.hpp`
- [ ] 节点类继承 `rclcpp::Node`，构造函数注入参数
- [ ] 至少 4 个可调参数（如 `front_sector_deg` / `stop_distance` / `cruise_speed` / `turn_speed`）通过 ROS2 参数系统暴露
- [ ] `colcon build --packages-select robot_behavior` 通过，无 warning
- [ ] **至少 1 个 GTest 单元测试**：测试避障逻辑（输入构造的 LaserScan → 期望 cmd_vel），`colcon test --packages-select robot_behavior` 通过
- [ ] launch 文件：`launch/behavior.launch.py` 可单独启动该节点
- [ ] 端到端验证：开仿真 + 启该节点，机器人能自主前进，遇障停下/转向
- [ ] PROJECT_STATE.md 中 M5 行标记 DONE，新增 Session 日志条目

## 产出物清单

| 路径 | 说明 |
|------|------|
| `src/robot_behavior/package.xml` | 依赖 rclcpp / sensor_msgs / geometry_msgs / ament_cmake_gtest |
| `src/robot_behavior/CMakeLists.txt` | ament_cmake + gtest |
| `src/robot_behavior/include/robot_behavior/obstacle_avoider.hpp` | 节点类声明 |
| `src/robot_behavior/src/obstacle_avoider_node.cpp` | 节点 main + 实现 |
| `src/robot_behavior/test/test_obstacle_avoider.cpp` | GTest 单元测试 |
| `src/robot_behavior/launch/behavior.launch.py` | 单独启动 launch |
| `src/robot_behavior/config/obstacle_avoider.yaml` | 默认参数 |

## 验证流程

```bash
# 0. build + test
cd ~/robot_ws
colcon build --packages-select robot_behavior
colcon test --packages-select robot_behavior
colcon test-result --verbose --test-result-base build/robot_behavior

# 1. 终端 A：仿真
source install/setup.bash
ros2 launch robot_bringup sim.launch.py

# 2. 终端 B：起避障节点
ros2 launch robot_behavior behavior.launch.py

# 3. 终端 C：观察 cmd_vel
ros2 topic echo /cmd_vel

# 4. 观察机器人在 Gazebo 中自主巡航 + 遇障行为
```

## 收尾动作

1. 更新 `PROJECT_STATE.md`：M5 行 DONE，产出清单更新，Session 日志追加
2. 回复用户："M5 DONE。请新开终端开启 M6 Session。"
3. **不要询问是否继续 M6**，不要主动开始 demo 录制

======
