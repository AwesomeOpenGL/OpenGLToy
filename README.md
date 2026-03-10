# OpenGLToy 构建方案总说明

## 1. 目标

这套工程采用 **CMake + vcpkg manifest mode** 管理第三方依赖，目标是把 OpenGLToy 组织成一套真正可迁移、可复现、可维护的工程化项目。

目标平台：

- Windows: `x64-windows`
- Linux: `x64-linux`

当前工程的直接依赖：

- `fmt`
- `glfw3`
- `glm`
- `stb`
- `glad[loader,gl-api-43]`

---

## 2. 这套方案解决了什么问题

传统的“小型 OpenGL 学习工程”通常有这些问题：

1. 第三方库路径写死在某台机器上。
2. 依赖库由人手工下载、手工编译、手工拷贝 include/lib。
3. Windows 与 Linux 的依赖组织方式不同，无法统一。
4. 切换机器、切换 IDE、切换生成器后，经常重新踩坑。
5. CMake 里充满硬编码路径，无法进入规模化工程阶段。

这套方案的核心就是把上面这些问题全部工程化：

- 依赖版本由 `vcpkg.json` 统一声明。
- 依赖获取由 vcpkg 自动执行。
- 依赖解析由 `find_package(...)` + imported targets 完成。
- 本工程源码仍然由 `add_executable(...)` 正常编译。
- Windows / Linux 通过同一套 CMake 模型收敛。

---

## 3. 目录结构与职责

```text
OpenGLToy/
├─ CMakeLists.txt
├─ CMakePresets.json
├─ vcpkg.json
├─ cmake/
│  └─ OpenGLToyDependencies.cmake
├─ scripts/
│  ├─ bootstrap-vcpkg.bat
│  ├─ bootstrap-vcpkg.sh
│  ├─ build-windows.bat
│  ├─ build-linux.sh
│  └─ setup-msvc-env.bat
├─ source/
└─ assets/
```

职责划分：

- `vcpkg.json`
  - 声明第三方依赖清单与版本基线。
- `CMakeLists.txt`
  - 定义本工程源码目标、编译选项、链接关系。
- `cmake/OpenGLToyDependencies.cmake`
  - 专门负责依赖解析与 target 兼容层。
- `scripts/*.bat / *.sh`
  - 负责工程自举、配置、编译入口。
- `CMakePresets.json`
  - 负责 CMake preset 标准化，方便命令行与 IDE 复用。

---

## 4. 原理总图：依赖库与源码如何“同时编译”

很多人会把这件事误解成：

> “CMake 一次性把第三方源码和本工程源码混在一个 Visual Studio 工程里一起编。”

这不是这套方案的真实机制。

### 真实机制分成两个层次

#### 层次 A：依赖准备层（由 vcpkg 管）

在 **configure 阶段**，`CMAKE_TOOLCHAIN_FILE` 指向 `vcpkg.cmake` 后，CMake 会先接入 vcpkg toolchain。此时如果工程是 manifest mode，vcpkg 会根据 `vcpkg.json` 自动执行依赖准备。

它做的事情包括：

1. 读取 `vcpkg.json`
2. 确定 triplet（如 `x64-windows` / `x64-linux`）
3. 安装或复用该 triplet 下的依赖
4. 将包的配置文件、头文件、库文件组织到：
   - `out/build/<preset>/vcpkg_installed/<triplet>/...`
   - 或 `vcpkg` 自身的已安装布局中

#### 层次 B：源码编译层（由 CMake 目标系统管）

当 vcpkg 准备好依赖后，`find_package(glfw3 CONFIG REQUIRED)`、`find_package(glm CONFIG REQUIRED)` 等调用就能找到对应包的 `Config.cmake`。

这些包不会简单返回“一个 lib 路径”，而是返回 **imported target**，例如：

- `fmt::fmt`
- `glm::glm`
- `glad::glad`
- `glfw` 或 `glfw3::glfw`

然后你的工程源码目标：

```cmake
add_executable(OpenGLToy ...)
target_link_libraries(OpenGLToy PRIVATE fmt::fmt glm::glm glad::glad ...)
```

这样一来，CMake 会在生成阶段建立一张“目标依赖图”：

- `OpenGLToy` 依赖哪些第三方目标
- 这些第三方目标需要哪些 include path
- 需要哪些 link library
- 是否有编译定义、传递依赖、系统库依赖

最后在 **build 阶段**：

- 编译的是 OpenGLToy 自己的 `.cpp`
- 链接的是 vcpkg 已经准备好的第三方库产物

所以这里的“同时编译”，更准确地说是：

> **同一条 configure/build 流程里，先自动准备依赖，再编译本工程源码，并在链接阶段把依赖目标接入。**

不是手工先编第三方、再手工改 include/lib；也不是必须把第三方源码直接 vendor 进来和主工程一个一个目标拼在一起。

---

## 5. CMake configure / generate / build 三个阶段到底在做什么

### 5.1 Configure 阶段

执行命令示例：

```bash
cmake -S . -B out/build/windows-Release \
  -DCMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=x64-windows
```

这个阶段主要做：

1. 读取 `CMakeLists.txt`
2. 接入 `vcpkg.cmake`
3. 解析 `vcpkg.json`
4. 自动安装 / 复用依赖
5. 执行 `find_package(...)`
6. 建立缓存 `CMakeCache.txt`
7. 准备生成器所需的构建描述

### 5.2 Generate 阶段

这个阶段把 CMake 的目标图转换成具体生成器可识别的构建文件：

- Visual Studio：`.vcxproj / .sln`
- Ninja：`build.ninja`
- Makefile：`Makefile`

### 5.3 Build 阶段

此阶段才真正调用编译器：

- `cl.exe` / `g++` / `clang++`
- 链接器
- 资源编译器

完成 `.cpp -> .obj/.o -> .exe` 的全过程。

---

## 6. 为什么这套方案不需要你手工设置 `glfw3-DIR`

这是你现在碰到的关键点。

### 结论先说

在 **vcpkg + CMake toolchain** 的标准工作流里，**你通常不应该手工设置 `glfw3_DIR`**。

### 原因

因为真正起作用的是：

- `CMAKE_TOOLCHAIN_FILE=<...>/vcpkg.cmake`
- `VCPKG_TARGET_TRIPLET=x64-windows` 或 `x64-linux`

当 toolchain 接入后，vcpkg 会把自己的包前缀路径注入到 CMake 的包搜索路径体系里，`find_package(glfw3 CONFIG REQUIRED)` 会自动去这些前缀里找 `glfw3Config.cmake`。

所以：

- `glfw3_DIR` 不是主入口
- `glfw3_DIR` 也不一定会出现在 CMake GUI 的缓存变量列表里
- 只有当 CMake 没找到 `glfw3`，或者项目特意把它暴露成 cache 变量时，你才更可能在 UI 中关注到它

### 更准确地说

CMake GUI 主要展示的是 **cache variables**。而 `glfw3_DIR` 并不是这套工程主动暴露的配置项；包一旦被 toolchain 正常解析，它往往不会成为你需要手工编辑的“入口变量”。

---

## 7. 在 CMake GUI / CMake UI 中正确配置本项目的方法

### 方法 A：命令行先走通，再用 IDE 打开构建目录

这是最稳的方法。

Windows：

```bat
scripts\build-windows.bat Debug
```

Linux：

```bash
./scripts/build-linux.sh Debug
```

构建目录一旦生成完成，IDE / CMake GUI 再去读取该目录即可。

### 方法 B：在 CMake GUI 中手工配置时，必须先设这两个变量

第一次 Configure 前，先添加：

- `CMAKE_TOOLCHAIN_FILE`
  - 值：`<你的工程根>/extern/vcpkg/scripts/buildsystems/vcpkg.cmake`
- `VCPKG_TARGET_TRIPLET`
  - Windows：`x64-windows`
  - Linux：`x64-linux`

Windows 若使用 VS 生成器，还要选：

- Generator: `Visual Studio 17 2022`
- Platform: `x64`

然后再点 Configure。

### 方法 C：直接用 CMake Presets

本工程已经提供：

- `windows-debug`
- `windows-release`
- `linux-debug`
- `linux-release`
- `windows-ninja-debug`
- `windows-ninja-release`

支持 preset 的 IDE 会直接识别这些 preset。这样你连手工填 toolchain 都不需要。

---

## 8. `glfw3-DIR` 真要手工填，应该填哪里

不推荐，但如果你一定要手工填，必须指向 **包含 `glfw3Config.cmake` 的目录**，而不是 `include/` 或 `lib/`。

典型位置类似：

```text
<build-dir>/vcpkg_installed/x64-windows/share/glfw3
```

或者：

```text
<vcpkg-root>/installed/x64-windows/share/glfw3
```

但再次强调：

> 在这套 manifest mode 工程里，正常情况下不需要手工设置它。

---

## 9. Windows 脚本的关键原理

### 9.1 为什么默认使用 `Visual Studio 17 2022`

因为 VS 生成器最稳定：

- 不需要你先手工开“Developer Command Prompt”
- CMake 更容易自动选中正确 SDK / toolset
- 对多数 Windows 用户更省心

### 9.2 为什么 Ninja 模式要单独准备 MSVC 环境

因为 Ninja 只是构建执行器，不负责帮你选择编译器环境。它要求当前 shell 已经能找到：

- `cl.exe`
- `rc.exe`
- `mt.exe`

否则 CMake 的 TryCompile 都会失败。

### 9.3 为什么使用 `cmake --fresh`

因为 CMake 一旦缓存了错误的编译器路径、错误的 toolchain 路径，后续再跑 configure 很可能继续复用错误缓存。`--fresh` 的作用就是把这些历史污染清掉。

---

## 10. Linux 脚本的关键原理

Linux 侧没有 VS 开发者环境这一层问题，逻辑更直：

1. 确保 `VCPKG_ROOT` 有效
2. 调用 CMake configure
3. 调用 CMake build

重点仍然是：

- `CMAKE_TOOLCHAIN_FILE`
- `VCPKG_TARGET_TRIPLET`

---

## 11. 依赖解析兼容层为什么要单独写一个 `OpenGLToyDependencies.cmake`

因为不同包的导出目标名并不总是一致。比如 `glfw3` 可能出现：

- `glfw`
- `glfw3::glfw`
- `glfw3::glfw3`

如果把这些判断全部散落在顶层 `CMakeLists.txt` 中，主工程会越来越脏。

所以这里专门做了一个兼容层：

- `find_package(...)`
- 依次检查候选 target
- 选到一个可链接的 target 后回传给顶层

这样主工程只关心：

```cmake
target_link_libraries(OpenGLToy PRIVATE ${OPENGLTOY_GLFW_TARGET})
```

---

## 12. 为什么 `GLFW_INCLUDE_NONE` 很关键

GLFW 默认可能会帮你 include 平台 OpenGL 头；但本工程使用的是 glad 作为 OpenGL loader。

正确方式是：

1. 先 include glad 头
2. 再 include GLFW 头

如果 GLFW 先把系统 OpenGL 头带进来，再 include glad，就会出现：

```text
#error: OpenGL header already included, remove this include, glad already provides it
```

所以必须在编译定义中加：

```cmake
GLFW_INCLUDE_NONE
```

---

## 13. 当前方案的边界

这套方案现在已经把“工程化构建系统”搭好了，但你原始压缩包仍然只包含 `source/`，资源目录内容并不完整。

因此当前状态是：

- CMake / vcpkg / 编译流程可以打通
- 代码可编译
- 但部分 sample 运行时可能还会因为 shader / texture 资源缺失而失败

这属于运行资源完整性问题，不是构建系统问题。

---

## 14. 推荐使用顺序

### Windows

```bat
scripts\build-windows.bat Debug
scripts\build-windows.bat Release
```

### Linux

```bash
chmod +x scripts/*.sh
./scripts/build-linux.sh Debug
./scripts/build-linux.sh Release
```

### 需要 IDE / GUI 时

优先使用：

- CMake Presets
- 或先命令行生成构建目录，再让 IDE 接管

不要一上来就在 GUI 中手工找 `glfw3_DIR`，那不是这套工程的主工作流。

---

## 15. 一句话总结

这套方案的核心不是“手工配置 glfw3 / glad / glm 的路径”，而是：

> **用 vcpkg manifest mode 在 configure 阶段自动准备依赖，再让 CMake 通过 imported targets 把这些依赖接入到本工程源码目标中。**

这才是现代 CMake 工程管理第三方库的正确方向。
