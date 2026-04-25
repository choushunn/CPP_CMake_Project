# CMake C++ VSCode 示例项目

这是一个在 **Windows + VSCode** 环境下使用 **CMake** 进行 C++ 多文件开发的最小示例。

## 功能说明

- 使用 CMake 管理多文件工程
- 使用 `include/` + `src/` 目录分离头文件与源文件
- 在 VSCode 中通过任务一键构建
- 在 VSCode 中通过 GDB 一键调试
- 强制接入 vcpkg（示例使用 `fmt`）

## 目录结构

```text
test_vs/
├─ CMakeLists.txt
├─ README.md
├─ include/
│  ├─ math.h
│  └─ student.h
├─ src/
│  ├─ main.cpp
│  ├─ math.cpp
│  └─ student.cpp
├─ .vscode/
│  ├─ settings.json
│  ├─ c_cpp_properties.json
│  ├─ tasks.json
│  ├─ launch.json
│  └─ extensions.json
└─ build/                 # CMake 构建输出目录（自动生成）
```

## 环境要求

- Windows 10/11
- VSCode
- CMake（建议 3.16+）
- GCC/G++ 与 GDB（如 MSYS2 UCRT64 工具链）
- vcpkg（必需，用于第三方库管理）
- VSCode 扩展：
  - C/C++（Microsoft）
  - CMake Tools（Microsoft）

> 示例默认使用 `C:/Programs/msys64/ucrt64/bin/` 下的编译器与调试器路径，请按你的本机实际安装路径调整。

## 关键安装信息与命令

### 1) 安装 VSCode 与 CMake

```powershell
winget install --id Microsoft.VisualStudioCode -e
winget install --id Kitware.CMake -e
```

安装后可验证：

```powershell
code --version
cmake --version
```

### 2) 安装 MSYS2 与 UCRT64 工具链

1. 下载安装 MSYS2（默认安装到 `C:\msys64` 或你自定义目录）。
2. 打开 **MSYS2 UCRT64** 终端，执行：

```bash
pacman -Syu
pacman -Su
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

3. 验证工具链：

```bash
g++ --version
gdb --version
cmake --version
mingw32-make --version
```

> 如果你使用的是本项目当前配置，请确保 `g++.exe`、`gdb.exe` 位于 `C:/Programs/msys64/ucrt64/bin/`（或将配置改成你的实际安装路径）。

### 3) 安装 VSCode 插件（两种方式）

- 方式 A：打开项目后，VSCode 会读取 `.vscode/extensions.json` 并提示安装推荐插件。
- 方式 B：命令行安装：

```powershell
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
code --install-extension twxs.cmake
code --install-extension ms-vscode.cpptools-extension-pack
```

### 4) Windows 终端 UTF-8 建议

```powershell
chcp 65001
```

如需长期生效，可在 PowerShell 配置中设置 UTF-8，或使用 VSCode 配置中的 `terminal.integrated.encoding = utf8`（本项目已配置）。

### 5) 安装 vcpkg（必需）

推荐使用 Git 安装：

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\Programs\vcpkg-2025.04.09
```

首次初始化（PowerShell）：

```powershell
C:\Programs\vcpkg-2025.04.09\bootstrap-vcpkg.bat
```

设置环境变量（当前终端会话）：

```powershell
$env:VCPKG_ROOT = "C:\Programs\vcpkg-2025.04.09"
$env:VCPKG_FORCE_DOWNLOADED_BINARIES = "1"
```

可验证：

```powershell
& "$env:VCPKG_ROOT\vcpkg.exe" version
```

## 快速开始

### 1) 命令行构建（必须使用 vcpkg）

项目根目录已提供：

- `vcpkg.json`：vcpkg manifest（声明 `fmt` 依赖）
- `CMakePresets.json`：提供 `vcpkg` 预设（强制走 vcpkg toolchain）
- 预设中已固定 `VCPKG_TARGET_TRIPLET=x64-mingw-dynamic`，匹配 MSYS2 MinGW 编译器 ABI
- 预设显式指定 `gcc/g++`，减少工具链探测失败概率

执行：

```powershell
$env:VCPKG_ROOT = "C:\Programs\vcpkg-2025.04.09"
$env:VCPKG_FORCE_DOWNLOADED_BINARIES = "1"
cmake --preset vcpkg
cmake --build --preset build
```

运行：

```powershell
.\build\demo.exe
```

若看到程序首行类似 `vcpkg fmt 已启用`，说明 vcpkg 依赖已生效。

### 2) 运行程序

```powershell
.\build\demo.exe
```

### 3) VSCode 中构建

- 打开项目根目录
- 按 `Ctrl+Shift+B`
- 选择（或默认使用）`CMake: Build`

### 4) VSCode 中调试

- 按 `F5`
- 选择 `Debug demo (GDB)`
- 调试前会自动执行 `CMake: Build`

## 关键配置说明

### `CMakeLists.txt`

- 指定最小 CMake 版本
- 设置项目语言与 C++ 标准
- 通过编译选项统一源码与执行字符集为 UTF-8（Windows 下中文更稳定）
- 添加头文件搜索路径
- 收集 `src/` 中 `.cpp` 源文件并生成可执行文件 `demo`
- 强制使用 `find_package(fmt CONFIG REQUIRED)` 并链接 `fmt::fmt`

### `vcpkg.json`

- 使用 manifest 模式声明第三方依赖（本示例为 `fmt`）
- 配合 CMake toolchain 可在配置阶段自动安装/解析依赖

### `CMakePresets.json`

- `vcpkg`：设置 `CMAKE_TOOLCHAIN_FILE`，构建时必须依赖 vcpkg
- `build`：对应 `vcpkg` 的构建流程
- `VCPKG_TARGET_TRIPLET=x64-mingw-dynamic`：确保第三方库与 `g++.exe` 工具链一致

### `.vscode/*.json` 文件作用总览

- `settings.json`：工作区级别的 VSCode 设置，控制 CMake 生成器、构建目录、终端编码、默认编译器等基础行为。
- `c_cpp_properties.json`：C/C++ 扩展的 IntelliSense 配置，影响代码补全、跳转、宏识别、头文件搜索路径等“编辑器体验”。
- `tasks.json`：任务系统配置，定义可复用的命令任务（如 `CMake: Configure`、`CMake: Build`），可通过 `Ctrl+Shift+B` 触发。
- `launch.json`：调试配置，定义按 `F5` 时如何启动程序、用哪个调试器、调试前是否执行预构建任务。

它们的关系可以理解为：

- `settings.json` 负责“全局行为偏好”
- `tasks.json` 负责“怎么编译”
- `launch.json` 负责“怎么调试”
- `c_cpp_properties.json` 负责“怎么写代码更智能”

### `.vscode/tasks.json`

- `CMake: Configure`：生成构建系统
- `CMake: Build`：执行实际编译（默认构建任务）

### `.vscode/launch.json`

- 使用 `cppdbg + gdb` 调试 `build/demo.exe`
- `preLaunchTask` 绑定 `CMake: Build`，保证调试前先编译

### `.vscode/settings.json`

- `cmake.generator`：指定 CMake 生成器（本示例使用 `MinGW Makefiles`）
- `cmake.buildDirectory`：指定构建目录（示例为 `${workspaceFolder}/build`）
- `cmake.configureOnOpen`：打开项目时是否自动执行 CMake 配置
- `C_Cpp.default.compilerPath`：C/C++ 扩展默认编译器路径
- `files.encoding`、`terminal.integrated.encoding`：编辑器和终端编码设置（本示例统一为 UTF-8）

### `.vscode/c_cpp_properties.json`

- `includePath`：IntelliSense 头文件搜索路径
- `defines`：预定义宏（如 `_WIN32`、`UNICODE`）
- `compilerPath`：用于推断系统头文件和编译器特性的编译器
- `cStandard`、`cppStandard`：语言标准（用于智能提示，不直接替代 CMake 编译标准）
- `intelliSenseMode`：选择对应平台/编译器的语义分析模式

## 可选扩展

如果你想继续规范化 C++ 结构：

- 头文件从 `*.h` 迁移为 `*.hpp`（可选）
- 按模块拆分为静态库/动态库再链接主程序
- 在 CMake 中增加测试目标（如 `ctest`）

## 常见问题

### 1) 终端中文乱码

可能是终端编码和编译期字符集不一致导致。建议：

- 源码统一保存为 UTF-8
- 终端编码、VSCode 文件编码、编译参数保持一致（推荐统一 UTF-8）
- Windows 终端若仍乱码，可先执行 `chcp 65001`

### 2) 找不到 `gdb.exe`

检查 `.vscode/launch.json` 中 `miDebuggerPath` 是否正确。

### 3) 构建器不匹配

若本机未安装 `MinGW Makefiles` 相关工具，可在 `.vscode/settings.json` 调整 `cmake.generator`，或改用 Ninja。

### 4) C++ 编译在 `main.cpp.obj` 阶段直接失败（无详细错误）

若出现如下报错：

- `Building CXX object ... main.cpp.obj`
- `mingw32-make ... Error 1`

通常是 `PATH` 中存在冲突 DLL（例如被其他软件目录抢先）。本示例已在以下配置中将 `C:/Programs/msys64/ucrt64/bin` 置于 PATH 前缀：

- `.vscode/settings.json`（终端环境）
- `.vscode/tasks.json`（Configure/Build 任务）
- `.vscode/launch.json`（调试环境）

若仍异常，请重启 VSCode 后再试，并在终端执行：

```powershell
chcp 65001
$env:VCPKG_ROOT = "C:\Programs\vcpkg-2025.04.09"
$env:VCPKG_FORCE_DOWNLOADED_BINARIES = "1"
cmake --preset vcpkg
cmake --build --preset build
```

### 5) vcpkg 检测编译器失败（`detect_compiler` / `System is unknown to cmake`）

若日志中出现 `buildtrees/detect_compiler` 失败，且包含 `System is unknown to cmake`，通常是 vcpkg 调用了系统 CMake（如 4.x）与 `x64-mingw-dynamic` 探测流程不兼容。建议在当前终端执行：

```powershell
$env:VCPKG_ROOT = "C:\Programs\vcpkg-2025.04.09"
$env:VCPKG_FORCE_DOWNLOADED_BINARIES = "1"
cmake --preset vcpkg
cmake --build --preset build
```

本项目的 `.vscode/tasks.json` 与 `.vscode/settings.json` 已内置上述变量，通常可直接 `Ctrl+Shift+B`。

## 许可证

此示例仅用于学习与演示，可自由修改使用。

