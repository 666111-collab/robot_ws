# PROJECT_STATE.md — 当前进度仪表盘

> 最后更新：2026-06-02
> 更新者：Session 结束时由 Claude 更新

---

## 当前状态

- **当前模块**：M6 DONE → **项目交付完成**
- **整体进度**：6/6 模块完成
- **演示物料**：以"1 动态 GIF + 5 截图"交付（覆盖 仿真/建图中/建图完成/导航/避障/巡航 全流程），已嵌入 README 门面与 §5；如需完整连续视频，按 `docs/demo_script.md` 分镜录制即可（可选增强）

---

## 模块进度总览

| 模块 | 状态 | 备注 |
|------|------|------|
| M1 环境搭建 | DONE | 所有包已装，.bashrc 已配置 |
| M2 工程骨架 | DONE | robot_bringup 包，sim.launch.py 可用 |
| M3 SLAM | DONE | robot_slam 包（lifecycle launch），maps/turtlebot3_world.{pgm,yaml} 已存 |
| M4 Nav2 | DONE | robot_navigation 包，nav.launch.py + nav2_params.yaml（TB3 burger 模板），RViz 验证 goal + 动态避障通过 |
| M5 C++ 节点 | DONE | robot_behavior 包；obstacle_aware_cruise 节点（rclcpp，RAII/smart ptr，4 参数），库/可执行分离，4 个 GTest 全过+lint 通过，仿真端到端验证巡航+遇障停转 |
| M6 Demo + README | DONE | README.md / docs/architecture.md(mermaid) / docs/demo_script.md / .gitignore 完成，命令 walk-through 验证可复现；demo.mp4 待用户按分镜录制 |

> 图例：PENDING / WIP / DONE / BLOCKED

---

## M1 详细状态

### 已完成
- [x] Ubuntu 24.04 环境
- [x] `ros-jazzy-desktop` 已安装
- [x] `~/robot_ws/` workspace 目录已创建
- [x] `~/robot_ws/src/` 已创建（空）
- [x] 项目协同三件套（CLAUDE.md / PROJECT_STATE.md / DECISIONS.md）

### 待补
- [x] 安装 `ros-jazzy-ros-gz`（Gazebo Harmonic + ROS-GZ Bridge）
- [x] 安装 `ros-jazzy-turtlebot3` `ros-jazzy-turtlebot3-simulations`
- [x] `.bashrc` 添加 `source /opt/ros/jazzy/setup.bash`
- [x] `.bashrc` 添加 `export TURTLEBOT3_MODEL=burger`
- [x] 验证命令通过：Gazebo 打开，TurtleBot3 burger 出现在六边形竞技场中

### 已知问题
- 旧的 `ros-jazzy-gazebo-msgs`（Gazebo Classic 残留）已安装，**不影响 Harmonic，不要卸载**

---

## 产出清单

| 路径 | 来源模块 | 状态 |
|------|---------|------|
| `CLAUDE.md` | 项目级 | DONE |
| `PROJECT_STATE.md` | 项目级 | DONE |
| `DECISIONS.md` | 项目级 | DONE |
| `src/robot_bringup/` | M2 | DONE |
| `src/robot_slam/` | M3 | DONE |
| `src/robot_navigation/` | M4 | DONE |
| `src/robot_behavior/` | M5 | DONE |
| ├ `package.xml` / `CMakeLists.txt` | M5 | DONE（库 obstacle_avoider_lib + 可执行 obstacle_aware_cruise + gtest） |
| ├ `include/robot_behavior/obstacle_avoider.hpp` | M5 | DONE（CruiseParams + computeCommand + 节点类声明） |
| ├ `src/obstacle_avoider.cpp` | M5 | DONE（纯避障逻辑） |
| ├ `src/obstacle_avoider_node.cpp` | M5 | DONE（节点+回调+main） |
| ├ `test/test_obstacle_avoider.cpp` | M5 | DONE（4 GTest 用例全过） |
| ├ `launch/behavior.launch.py` | M5 | DONE |
| └ `config/obstacle_avoider.yaml` | M5 | DONE（4 可调参数） |
| `maps/turtlebot3_world.pgm` | M3 | DONE（112×103，11.5KB） |
| `maps/turtlebot3_world.yaml` | M3 | DONE |
| `media/nav2_goal.png` | M4 | DONE |
| `media/nav2_avoid.png` | M4 | DONE |
| `docs/architecture.md` | M6 | DONE（mermaid：包拓扑 + 导航 topic 流 + C++ 节点内部数据流） |
| `docs/demo_script.md` | M6 | DONE（9 场次分镜，≈83s，含 ffmpeg 拼接/转 gif） |
| `README.md` | M6 | DONE（7 section 全，命令验证可复现） |
| `.gitignore` | M6 | DONE（排除 build/install/log） |
| `media/01_gazebo_world.png` `02_slam_mapping.png` `03_slam_map_complete.png` | M6 | DONE（用户采集，已重命名为语义名并嵌入 README） |
| `media/cruise_demo.gif` | M6 | DONE（动态巡航 GIF，README 门面 hero） |
| `media/demo.mp4`（完整连续视频） | M6 | 可选增强（截图+GIF 已满足展示，按 demo_script.md 可补录） |

---

## 下一步动作

开 M6 Session：

```
Session: M6-Demo+README
读 CLAUDE.md / PROJECT_STATE.md / DECISIONS.md
继续上次进度
```

M6 目标：docs/（架构图、demo 脚本）、media/demo.mp4、README.md（面试展示）。
可串联 M3 建图 → M4 Nav2 导航 → M5 兜底避障 讲一条完整故事线。

---

## Session 日志

| 日期 | Session | 主要动作 |
|------|---------|---------|
| 2026-05-24 | 初始化 | 创建 CLAUDE.md / PROJECT_STATE.md / DECISIONS.md |
| 2026-05-24 | 初始化 | 验证 M1 环境，发现 Gazebo Harmonic / TB3 包未装 |
| 2026-05-25 | M1 | 安装 ros-jazzy-ros-gz / turtlebot3 / turtlebot3-simulations，配置 .bashrc，M1 DONE |
| 2026-05-25 | M2 | 创建 robot_bringup 包，sim.launch.py 一键启动仿真+RViz2，colcon build 通过，M2 DONE |
| 2026-05-26 | M3 | 创建 robot_slam 包；slam_toolbox.yaml + slam.launch.py（LifecycleNode + configure/activate）+ slam.rviz；首次建图发现普通 Node 无法触发 lifecycle 转换导致参数不加载，照官方 online_async_launch.py 改为 LifecycleNode 模式后修复；teleop 建图保存 maps/turtlebot3_world.{pgm,yaml}（112×103，11.5KB），M3 DONE |
| 2026-05-27 | M4 | 创建 robot_navigation 包；nav2_params.yaml 直接复用 turtlebot3_navigation2/param/burger.yaml（TwistStamped cmd_vel 与 TB3 Harmonic bridge 匹配）；nav.launch.py 包装 nav2_bringup/bringup_launch.py 并默认加载 M3 地图 + nav2_default_view.rviz；冒烟启动所有 lifecycle 节点（map_server / amcl / controller / planner / smoother / behavior / bt_navigator / waypoint_follower / velocity_smoother / collision_monitor）成功 activated；RViz 2D Pose Estimate + Nav2 Goal 验证到位 + Gazebo spawn box 验证 replan 绕障，截图 media/nav2_goal.png / media/nav2_avoid.png，M4 DONE |
| 2026-06-02 | M6 | 产出全部展示物料：README.md（7 section，中文+英文术语，含 mermaid 架构缩略图、apt/build/launch 快速开始、建图/导航/避障三段演示、亮点、roadmap）；docs/architecture.md（3 张 mermaid：包拓扑 / 导航 topic 流 / C++ 节点内部数据流 + 参数表）；docs/demo_script.md（9 场次分镜≈83s + ffmpeg 拼接/转 gif + 面试口播总纲）；新增 .gitignore。命令 walk-through 验证：4 个 launch 文件均可发现、obstacle_aware_cruise 可执行存在、teleop/map_saver 依赖在位、默认地图在位，README 快速开始全部可复现。demo.mp4 按 M6 约束由用户自行录制（Claude 只给分镜），为唯一剩余人工动作。M6 DONE，项目 6/6 交付完成 |
| 2026-06-01 | M5 | 创建 robot_behavior 包，教学式结对完成 obstacle_aware_cruise 节点：继承 rclcpp::Node，4 个 declare_parameter 可调参数，RAII+SharedPtr 全程无裸 new/delete，订阅 /scan(SensorDataQoS)→computeCommand 纯逻辑→发布 /cmd_vel(TwistStamped 贴 now()戳)；工程上拆库(obstacle_avoider_lib 纯逻辑)+可执行，4 个 GTest 用例（直行/停转/侧方忽略/inf-nan 过滤）全过；踩坑 ament_copyright 不认 SPDX 简写需完整 Apache 文本块、cpplint include_order 把 gtest 当 C 头，均修至无 warning；端到端验证：干净仿真下 30s 采样 84 条 cmd_vel，76 条巡航(0.15/0.0)+8 条遇障停转(0.0/0.6)，odom 证实真实位移，决策记 D010/D011，M5 DONE |
