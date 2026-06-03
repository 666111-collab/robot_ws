# DECISIONS.md — 关键技术决策记录

> 追加式，不删除（保留历史推演）。
> 每条格式：编号 + 主题 + 日期 + 选项 + 选择 + 理由。

---

## D001 — ROS2 版本

- **日期**：2026-05-24
- **选项**：Humble / Iron / Jazzy
- **选择**：Jazzy
- **理由**：
  - Ubuntu 24.04 官方搭档版本
  - LTS（维护到 2029）
  - 与 Gazebo Harmonic 配套

---

## D002 — 仿真器

- **日期**：2026-05-24
- **选项**：Gazebo Classic（gazebo-11，已 EOL）/ Gazebo Harmonic（gz-sim）
- **选择**：Gazebo Harmonic
- **理由**：
  - Jazzy 官方推荐搭档（REP-2000）
  - Classic 已 EOL（2025-01）
  - 通过 `ros-jazzy-ros-gz` 一键集成

---

## D003 — 机器人平台

- **日期**：2026-05-24
- **选项**：TurtleBot3 burger / waffle / waffle_pi / 自定义
- **选择**：TurtleBot3 burger
- **理由**：
  - 最轻，仿真启动最快
  - 官方 Jazzy 包齐全
  - 自带 LiDAR，demo 够用

---

## D004 — 核心节点语言

- **日期**：2026-05-24
- **选项**：Python（rclpy）/ C++（rclcpp）
- **选择**：C++17（rclcpp）
- **理由**：
  - 面试硬性要求
  - 体现 Modern C++（RAII / smart pointer / lifecycle）
  - rclcpp 是具身智能公司主流

---

## D005 — SLAM 方案

- **日期**：2026-05-24
- **选项**：slam_toolbox / cartographer / rtabmap
- **选择**：slam_toolbox
- **理由**：
  - Nav2 官方推荐
  - Jazzy apt 包开箱即用
  - 2D 场景最稳，TurtleBot3 标配

---

## D006 — 导航栈

- **日期**：2026-05-24
- **选项**：Nav2 / 自写 / move_base（ROS1 遗留）
- **选择**：Nav2（nav2_bringup）
- **理由**：
  - ROS2 唯一主流方案
  - TurtleBot3 已有官方 Nav2 配置
  - 只调参数 yaml，不重写 planner

---

## D007 — 项目协同 workflow

- **日期**：2026-05-24
- **选项**：单 Session 通做 / 按模块拆 Session
- **选择**：按 6 模块拆 Session
- **理由**：
  - 避免 context window 压缩失真
  - 单模块崩了不污染其他模块
  - 通过 PROJECT_STATE.md 传递状态

---

## D008 — 项目范围裁剪

- **日期**：2026-05-24
- **砍掉**：
  - YOLO / 视觉感知
  - 自定义机器人 / URDF
  - ROS2 Control / MoveIt
  - 真机部署
- **保留**：
  - Gazebo 仿真
  - LiDAR + Camera 话题接入
  - SLAM 建图
  - Nav2 导航
  - 基础避障
  - 至少 1 个 Modern C++ 节点
- **理由**：3 天工期 + 聚焦"移动机器人导航"主线

---

## D009 — 目标公司方向

- **日期**：2026-05-24
- **选择**：通用，暂不锁定
- **影响**：
  - README / demo talking points 走通用风格
  - 后续如锁定方向（人形 / AMR / VLA），追加 D010 调整 demo 重点

---

## D010 — M5 节点 /cmd_vel 消息类型

- **日期**：2026-06-01
- **选项**：`geometry_msgs/msg/Twist` / `geometry_msgs/msg/TwistStamped`
- **选择**：TwistStamped
- **理由**：
  - TB3 burger 的 Gazebo Harmonic bridge 配置 `turtlebot3_burger_bridge.yaml` 里 `ros_type_name: geometry_msgs/msg/TwistStamped`，发普通 Twist 机器人不动
  - M4 nav2_params.yaml 全程 `enable_stamped_cmd_vel: true`，与之一致
  - M5 任务书原写"发布 Twist"，与真实环境冲突，按实际 bridge 改为 TwistStamped
- **实现**：纯逻辑 `computeCommand` 返回无戳 `Twist`（逻辑层不碰 ROS 细节），节点回调里包成 `TwistStamped` 并贴 `now()` 时间戳 + `base_link` frame 再发布

---

## D011 — M5 避障节点工程结构（库/可执行分离）

- **日期**：2026-06-01
- **选项**：逻辑直接写进 subscription callback / 拆出纯函数库
- **选择**：拆库——`obstacle_avoider_lib`(纯逻辑 computeCommand) + `obstacle_aware_cruise`(节点+main)
- **理由**：
  - 纯函数 `computeCommand(scan, params)->Twist` 不依赖 spin/IO，GTest 可直接喂构造的 LaserScan 断言输出，无需起节点
  - 单测覆盖 4 分支：空旷直行 / 正前障碍停转 / 侧方障碍忽略 / inf-nan 无效值过滤
  - 符合 ROS2 可测试节点标准实践

---

## D012 — 项目完结，交付物总览

- **日期**：2026-06-02
- **背景**：M6 收尾，6/6 模块完成，定格最终交付边界
- **交付物**：
  - **代码**：4 个 ROS2 包 `robot_bringup`(仿真) / `robot_slam`(建图) / `robot_navigation`(Nav2) / `robot_behavior`(Modern C++ 避障节点 + 4 GTest)
  - **资产**：`maps/turtlebot3_world.{pgm,yaml}`、`media/nav2_goal.png`、`media/nav2_avoid.png`
  - **文档**：`README.md`(7 section 门面)、`docs/architecture.md`(3 张 mermaid)、`docs/demo_script.md`(分镜)、`DECISIONS.md`、`PROJECT_STATE.md`、`CLAUDE.md`、`.gitignore`
- **故事线**：仿真启动 → SLAM 建图 → Nav2 自主导航 → 动态避障 → C++ 兜底巡航，全程 `ros2 launch` 一键复现
- **范围守住**：未越界做 YOLO/视觉/自定义 URDF/MoveIt/真机/多机（按 D008 砍除项，仅写入 README roadmap）
- **唯一剩余人工动作**：用户按 `docs/demo_script.md` 用 OBS/SimpleScreenRecorder 录制 `media/demo.mp4`（M6 约束：录屏由用户完成，Claude 只提供脚本）

---

（新决策追加到下方）
