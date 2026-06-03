# M3-SLAM Session 启动提示词

> 复制以下 ====== 之间的内容作为新 Session 的第一句话。

======

Session: M3-SLAM 建图

先读 `~/robot_ws/CLAUDE.md` / `~/robot_ws/PROJECT_STATE.md` / `~/robot_ws/DECISIONS.md`，严格按其中规则执行。

## 你的角色
SLAM 工程师。**只负责 M3 模块**，不要碰 M4 / M5 / M6 的任何事，不要修改其他模块的文件。

## 模块目标
在 Gazebo 中用 slam_toolbox 建出 `turtlebot3_world` 的完整地图，保存为 pgm + yaml。

## 技术约束（不要替换方案）
- SLAM 方案：**slam_toolbox**（online_async 模式）
- 仿真：Gazebo Harmonic + TurtleBot3 burger
- 地图保存：`nav2_map_server` 的 `map_saver_cli`
- 不允许讨论 cartographer / rtabmap
- 不允许自写 SLAM 算法
- 包只创建 `src/robot_slam/`，不动 `robot_bringup`

## Definition of Done（不满足任何一条不允许标 DONE）

- [ ] `src/robot_slam/` 包结构完整：`package.xml` / `CMakeLists.txt` / `launch/slam.launch.py` / `config/slam_toolbox.yaml`
- [ ] `colcon build --packages-select robot_slam` 通过，无 warning 关键报错
- [ ] `ros2 launch robot_slam slam.launch.py` 能启动并订阅 `/scan`
- [ ] 用 `turtlebot3_teleop teleop_keyboard` **人工驾驶机器人扫完整张 turtlebot3_world**，回环闭合（RViz 中地图边界完整、无明显错层）
- [ ] `~/robot_ws/maps/turtlebot3_world.pgm` 存在，文件大小 > 10KB
- [ ] `~/robot_ws/maps/turtlebot3_world.yaml` 存在且字段正确（image / resolution / origin / occupied_thresh / free_thresh / negate）
- [ ] PROJECT_STATE.md 中 M3 行标记 DONE，新增 M3 Session 日志条目

## 产出物清单（必须落到磁盘）

| 路径 | 说明 |
|------|------|
| `src/robot_slam/package.xml` | 包声明，依赖 slam_toolbox / nav2_map_server |
| `src/robot_slam/CMakeLists.txt` | ament_cmake |
| `src/robot_slam/launch/slam.launch.py` | 启动 slam_toolbox online_async + RViz2 SLAM 配置 |
| `src/robot_slam/config/slam_toolbox.yaml` | SLAM 参数（已存在，按需调整） |
| `maps/turtlebot3_world.pgm` | 地图栅格图 |
| `maps/turtlebot3_world.yaml` | 地图元数据 |

## 验证流程（必须真正跑通）

```bash
# 0. build
cd ~/robot_ws && colcon build --packages-select robot_slam && source install/setup.bash

# 1. 终端 A：启动仿真
ros2 launch robot_bringup sim.launch.py

# 2. 终端 B：启动 SLAM
ros2 launch robot_slam slam.launch.py

# 3. 终端 C：键盘 teleop
ros2 run turtlebot3_teleop teleop_keyboard
# → 人工驾驶机器人扫完整张地图，注意回到起点形成闭环

# 4. 终端 D：保存地图
ros2 run nav2_map_server map_saver_cli -f ~/robot_ws/maps/turtlebot3_world

# 5. 验证产出
ls -la ~/robot_ws/maps/
file ~/robot_ws/maps/turtlebot3_world.pgm
cat ~/robot_ws/maps/turtlebot3_world.yaml
```

## 收尾动作

1. 更新 `PROJECT_STATE.md`：M3 行改 DONE，产出清单更新，Session 日志追加一行
2. 回复用户："M3 DONE。请新开终端开启 M4 Session。"
3. **不要询问是否继续 M4**，不要主动提出帮做 M4 的任何事

======
