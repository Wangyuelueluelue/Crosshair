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
