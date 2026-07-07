# Crosshair

一个基于纯 C 语言和 Win32 API 编写的极简 FPS 游戏十字准星叠加层。

## 🌟 特性
- **极致轻量**：独立单文件，体积仅 100KB 左右。
- **零依赖**：无需安装 Python、.NET 或任何运行时库，双击即用。
- **绝对穿透**：底层 API 实现完美的鼠标事件穿透，绝不抢夺游戏焦点。
- **极低占用**：纯原生 C 代码，后台内存占用 < 1MB，不影响游戏帧数。

## 🚀 如何使用
前往 [Releases] 页面下载最新版的 `Crosshair.exe`，双击运行即可。右下角系统托盘可切换准星颜色或退出。

## 🛠️ 编译指南 (针对开发者)
本项目使用 MinGW (GCC) 编译，包含资源文件打包：
1. 编译图标资源：`windres icon.rc -O coff -o icon.res`
2. 编译主程序：`gcc main.c icon.res -o Crosshair.exe -mwindows -fexec-charset=GBK`

## 💡 创作初衷
作为一名硬核生存游戏玩家，我在游玩《SCUM》等游戏时遇到了一个极大的痛点：当角色在奔跑或进行某些战术动作时，原生的准星往往会消失。这在遭遇突发交火时非常致命。为了解决这个问题并保持屏幕视野的纯净，我开发了这款极简准星工具。它同样完美适用于《Rust》、《CS2》或任何需要稳定中心基准点的 FPS 射击游戏。

## ⚠️ 游戏设置须知 (非常重要)
**请务必将你的游戏画面设置为“无边框全屏 (Borderless / Windowed Fullscreen)”模式。**
如果游戏处于“独占全屏 (Exclusive Fullscreen)”模式，游戏引擎将垄断显卡输出通道，导致操作系统的所有置顶窗口（包括本程序的准星）被强制遮挡。开启无边框模式即可完美叠加准星，且几乎不影响游戏帧数。

## 💡 Motivation
As a hardcore survival game player, I encountered a major pain point while playing *SCUM*: the native crosshair often disappears completely when the character is running or performing certain tactical movements. This can be fatal during sudden firefights. To solve this issue while keeping the screen UI clean, I developed this minimalist crosshair tool. It is also highly effective for *Rust*, *CS2*, or any FPS games that require a persistent center reference point.

## ⚠️ Important Game Settings
**You MUST set your game's display mode to "Borderless" or "Windowed Fullscreen".**
If your game is running in "Exclusive Fullscreen" mode, the game engine takes complete control of the GPU output, which will forcibly hide all top-most Windows applications (including this crosshair overlay). Setting the game to Borderless mode ensures the crosshair displays perfectly without sacrificing your framerate.
