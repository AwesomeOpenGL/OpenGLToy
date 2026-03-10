# CMake GUI / CMake UI 下使用 OpenGLToy 的说明

## 为什么你看不到 glfw3-DIR

因为本工程采用的是 `vcpkg + CMAKE_TOOLCHAIN_FILE` 的工作流。

在这种模式下，`find_package(glfw3 CONFIG REQUIRED)` 主要依赖的是：

- `CMAKE_TOOLCHAIN_FILE`
- `VCPKG_TARGET_TRIPLET`

而不是依赖用户手工填写 `glfw3_DIR`。

因此在 CMake GUI 中：

- 看不到 `glfw3_DIR` 是正常现象
- 这不表示 `glfw3` 没有被使用
- 也不表示配置有问题

## 第一次配置时应该先设置什么

### Windows

- Source code: `你的工程根目录`
- Build directory: `你的工程根/out/build/windows-Debug` 或其他目录
- Generator: `Visual Studio 17 2022`
- Optional platform: `x64`

第一次 Configure 前，新增以下 cache 项：

- `CMAKE_TOOLCHAIN_FILE`
  - `你的工程根/extern/vcpkg/scripts/buildsystems/vcpkg.cmake`
- `VCPKG_TARGET_TRIPLET`
  - `x64-windows`

### Linux

- Generator: `Ninja`
- `CMAKE_TOOLCHAIN_FILE`
  - `你的工程根/extern/vcpkg/scripts/buildsystems/vcpkg.cmake`
- `VCPKG_TARGET_TRIPLET`
  - `x64-linux`

## 什么时候才需要关心 glfw3_DIR

只有当：

1. 你没有使用 vcpkg toolchain
2. 或者 package 查找失败
3. 或者你在做非标准布局的手工依赖接入

这时你才需要手工指定：

```text
<build-dir>/vcpkg_installed/x64-windows/share/glfw3
```

但对当前工程，这不是推荐路径。

## 推荐做法

优先使用：

```bat
scripts\build-windows.bat Debug
```

或者：

```bash
./scripts/build-linux.sh Debug
```

先让命令行把配置与依赖解析跑通，再交给 IDE / GUI 查看和维护。
