# M6-Demo + README Session 启动提示词

> 复制以下 ====== 之间的内容作为新 Session 的第一句话。

======

Session: M6-Demo + README

先读 `~/robot_ws/CLAUDE.md` / `~/robot_ws/PROJECT_STATE.md` / `~/robot_ws/DECISIONS.md`，严格按其中规则执行。

## 你的角色
Tech Lead + Demo 导演。**只负责 M6 模块**：产出面向通用具身智能公司的展示物料。
**不要再改任何 M1-M5 的代码**（除非发现致命 bug 必须修，否则不动）。

## 前置依赖检查
- [ ] M1-M5 全部 DONE
- [ ] `media/` 已有 nav2 截图（M4 产出）
- [ ] 所有 launch 可一键复现

## 模块目标
产出 3 件展示物料：
1. **README.md**：项目门面，让面试官 3 分钟看懂、5 分钟跑通
2. **docs/architecture.md**：mermaid 架构图 + 数据流
3. **media/demo.mp4**（或 .gif）：端到端 demo 录屏

## 技术约束
- README 主语言：中文（target 国内具身智能公司）+ 关键术语保留英文
- 架构图：**mermaid**（GitHub 原生渲染）
- 录屏工具：用户用 SimpleScreenRecorder / OBS / `wf-recorder` 自行录制；Claude 只提供录屏脚本（场次清单）
- **不引入新依赖、不重写代码**

## Definition of Done

- [ ] `README.md` 包含全部 7 个 section（见下方模板）
- [ ] `docs/architecture.md` 含 mermaid 图：节点拓扑 + topic 流
- [ ] `docs/demo_script.md` 含分镜脚本（每个场次：场景 / 命令 / 时长 / 解说点）
- [ ] `media/demo.mp4` 或 `media/demo.gif` 存在（≥ 60s，覆盖：sim 启动 / SLAM 建图 / Nav2 点目标 / 避障 / C++ 节点巡航）
- [ ] README 中所有命令在干净终端复制粘贴**能直接跑通**（Claude 至少口头 walk-through 一遍）
- [ ] PROJECT_STATE.md 整体进度更新为 6/6 DONE，新增 Session 日志条目

## README.md 必备 7 个 section

```
1. 项目简介（一段话 + 一个 gif）
2. 技术栈表格（ROS2 Jazzy / Gazebo Harmonic / TB3 burger / C++17）
3. 系统架构图（embed docs/architecture.md 里的 mermaid 或截图）
4. 快速开始（apt deps + clone + colcon build + 一键 launch）
5. 演示步骤（M3 建图 / M4 导航 / M5 避障 各一段命令）
6. 项目亮点（Modern C++ 设计 / 一键复现 / 工程化结构）
7. 后续可扩展方向（视觉感知 / VLA 集成 / 真机部署 — 写成 roadmap 而非已做）
```

## 产出物清单

| 路径 | 说明 |
|------|------|
| `README.md` | 项目门面 |
| `docs/architecture.md` | mermaid 架构图 + 数据流说明 |
| `docs/demo_script.md` | 录屏分镜脚本 |
| `media/demo.mp4` 或 `media/demo.gif` | 端到端演示视频 |
| `.gitignore` | 排除 build/ install/ log/（如还没有） |

## 验证流程

```bash
# 1. 干净终端复现 README "快速开始" 全部命令
cd ~/robot_ws && rm -rf build install log
colcon build && source install/setup.bash
# 然后按 README 一步步走，全部跑通

# 2. mermaid 渲染检查（在 GitHub / vscode-mermaid 预览）

# 3. demo 视频播放完整性检查
```

## 收尾动作

1. 更新 `PROJECT_STATE.md`：M6 行 DONE，整体进度 6/6 DONE
2. 在 `DECISIONS.md` 追加最终一条："D0xx — 项目完结，交付物总览"
3. 回复用户："M6 DONE，项目交付完成。"
4. **不要主动建议**"是否要做 v2 / 加 YOLO / 上传 GitHub" 等超出范围的事

======
