# M4-Nav2 Session 启动提示词

> 复制以下 ====== 之间的内容作为新 Session 的第一句话。

======

Session: M4-Nav2 导航

先读 `~/robot_ws/CLAUDE.md` / `~/robot_ws/PROJECT_STATE.md` / `~/robot_ws/DECISIONS.md`，严格按其中规则执行。

## 你的角色
导航工程师。**只负责 M4 模块**，依赖 M3 产出的地图。不要碰 M5 / M6，不要重做 M3。

## 前置依赖检查（开工前必查）
- [ ] `~/robot_ws/maps/turtlebot3_world.pgm` 和 `.yaml` 存在 → 不存在则报告"M3 未完成"并停止
- [ ] `src/robot_bringup/launch/sim.launch.py` 可启动

## 模块目标
基于 M3 地图，用 Nav2 实现：RViz2 点目标点 → TurtleBot3 自主规划路径并导航到位 + 动态避障。

## 技术约束
- 导航栈：**Nav2（nav2_bringup）**，只调 yaml 参数
- 不重写 planner / controller / behavior tree
- 不引入 MoveIt / ROS2 Control
- 包只创建 `src/robot_navigation/`

## Definition of Done（不满足任何一条不允许标 DONE）

- [ ] `src/robot_navigation/` 包结构完整：`package.xml` / `CMakeLists.txt` / `launch/nav.launch.py` / `config/nav2_params.yaml`
- [ ] `colcon build --packages-select robot_navigation` 通过
- [ ] `ros2 launch robot_navigation nav.launch.py map:=~/robot_ws/maps/turtlebot3_world.yaml` 能启动完整 Nav2 栈（amcl + planner + controller + bt + behavior + smoother）
- [ ] RViz2 中通过 "2D Pose Estimate" 设置初始位姿
- [ ] RViz2 中通过 "Nav2 Goal" 点击目标点，机器人**真的走过去到位**（误差 < 0.3m）
- [ ] **动态避障验证**：导航过程中在 Gazebo 里 spawn 一个新障碍物，机器人能 replan 绕过
- [ ] 至少 2 个截图保存到 `media/`：`media/nav2_goal.png` / `media/nav2_avoid.png`
- [ ] PROJECT_STATE.md 中 M4 行标记 DONE，新增 Session 日志条目

## 产出物清单

| 路径 | 说明 |
|------|------|
| `src/robot_navigation/package.xml` | 依赖 nav2_bringup / nav2_map_server / nav2_amcl |
| `src/robot_navigation/CMakeLists.txt` | ament_cmake |
| `src/robot_navigation/launch/nav.launch.py` | 启动 Nav2 + 加载 M3 地图 + RViz2 nav 配置 |
| `src/robot_navigation/config/nav2_params.yaml` | Nav2 参数（基于 nav2_bringup 官方模板修改） |
| `src/robot_navigation/rviz/nav.rviz` | RViz2 视图配置（可选） |
| `media/nav2_goal.png` | RViz 中点目标 + 路径规划截图 |
| `media/nav2_avoid.png` | 动态避障截图 |

## 验证流程

```bash
# 0. build
cd ~/robot_ws && colcon build --packages-select robot_navigation && source install/setup.bash

# 1. 终端 A：仿真
ros2 launch robot_bringup sim.launch.py

# 2. 终端 B：Nav2 + 地图
ros2 launch robot_navigation nav.launch.py map:=$HOME/robot_ws/maps/turtlebot3_world.yaml

# 3. RViz2 操作：
#   a. "2D Pose Estimate" 设初始位姿
#   b. "Nav2 Goal" 点目标点 → 观察规划路径 + 机器人移动
#   c. 在 Gazebo 中拖入一个 box 障碍物 → 观察 replan

# 4. 截图保存到 media/
```

## 收尾动作

1. 更新 `PROJECT_STATE.md`：M4 行 DONE，产出清单更新，Session 日志追加
2. 回复用户："M4 DONE。请新开终端开启 M5 Session。"
3. **不要询问是否继续 M5**，不要主动启动 M5 的任何事

======
