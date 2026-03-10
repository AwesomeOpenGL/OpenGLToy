cmake_minimum_required(VERSION 3.24)

project(OpenGLToy
    VERSION 0.1.0
    DESCRIPTION "OpenGL learning playground powered by CMake and vcpkg"
    LANGUAGES CXX)

# 统一 C++ 语言标准。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 工程开关：
# OPENGLTOY_ENABLE_WARNINGS      是否启用较严格的告警级别
# OPENGLTOY_WARNINGS_AS_ERRORS   是否把告警提升为错误
option(OPENGLTOY_ENABLE_WARNINGS "Enable strict compiler warnings." ON)
option(OPENGLTOY_WARNINGS_AS_ERRORS "Treat warnings as errors." OFF)

# 资源根目录作为 cache 变量暴露出来，方便 IDE / CMake GUI 覆盖。
set(OPENGLTOY_ASSET_ROOT "${PROJECT_SOURCE_DIR}/assets" CACHE PATH "Runtime asset root.")

# 单独封装依赖解析逻辑，保持顶层 CMakeLists 简洁。
include(cmake/OpenGLToyDependencies.cmake)
opengltoy_resolve_dependencies()

# 收集工程源码。这里仍然显式列文件，而不是 GLOB，
# 目的是保证新增/删除源码文件时，工程变更是“可见、可审计、可版本管理”的。
set(OPENGLTOY_SOURCES
    source/app/Application.cpp
    source/app/Application.h
    source/app/main.cpp
    source/foundation/Base/NonCopyable.h
    source/foundation/Diagnostics/Assert.cpp
    source/foundation/Diagnostics/Assert.h
    source/foundation/Diagnostics/Log.cpp
    source/foundation/Diagnostics/Log.h
    source/foundation/IO/FileSystem.cpp
    source/foundation/IO/FileSystem.h
    source/foundation/Time/HighResolutionClock.cpp
    source/foundation/Time/HighResolutionClock.h
    source/platform/Glfw/GlfwInputService.cpp
    source/platform/Glfw/GlfwInputService.h
    source/platform/Glfw/GlfwWindow.cpp
    source/platform/Glfw/GlfwWindow.h
    source/platform/IInputService.h
    source/platform/IWindow.h
    source/platform/WindowTypes.h
    source/render/OpenGL/GLBuffer.cpp
    source/render/OpenGL/GLBuffer.h
    source/render/OpenGL/GLCommon.h
    source/render/OpenGL/GLDebug.cpp
    source/render/OpenGL/GLDebug.h
    source/render/OpenGL/GLLoader.cpp
    source/render/OpenGL/GLLoader.h
    source/render/OpenGL/GLShaderProgram.cpp
    source/render/OpenGL/GLShaderProgram.h
    source/render/OpenGL/GLTexture2D.cpp
    source/render/OpenGL/GLTexture2D.h
    source/render/OpenGL/GLVertexArray.cpp
    source/render/OpenGL/GLVertexArray.h
    source/render/Resources/StbImageImplementation.cpp
    source/render/Scene/Camera.cpp
    source/render/Scene/Camera.h
    source/samples/BuiltInSampleRegistration.cpp
    source/samples/BuiltInSampleRegistration.h
    source/samples/ISample.h
    source/samples/SampleRegistry.cpp
    source/samples/SampleRegistry.h
    source/samples/getting_started/SampleCameraClass.cpp
    source/samples/getting_started/SampleCameraClass.h
    source/samples/getting_started/SampleCoordinateSystems.cpp
    source/samples/getting_started/SampleCoordinateSystems.h
    source/samples/getting_started/SampleHelloTriangle.cpp
    source/samples/getting_started/SampleHelloTriangle.h
    source/samples/getting_started/SampleHelloWindow.cpp
    source/samples/getting_started/SampleHelloWindow.h
    source/samples/getting_started/SampleShadersBasic.cpp
    source/samples/getting_started/SampleShadersBasic.h
    source/samples/getting_started/SampleTexturesBasic.cpp
    source/samples/getting_started/SampleTexturesBasic.h
    source/samples/getting_started/SampleTransformations.cpp
    source/samples/getting_started/SampleTransformations.h
)

# 定义主程序目标。
add_executable(OpenGLToy ${OPENGLTOY_SOURCES})
add_executable(OpenGLToy::App ALIAS OpenGLToy)

# Visual Studio / IDE 分组显示，让源码树更整洁。
source_group(TREE "${PROJECT_SOURCE_DIR}/source" PREFIX "Source Files" FILES ${OPENGLTOY_SOURCES})

set_target_properties(OpenGLToy PROPERTIES
    OUTPUT_NAME "OpenGLToy"
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

# 本工程私有头文件搜索路径：
#   - source/                    项目源码头
#   - OPENGLTOY_STB_INCLUDE_DIR  由 vcpkg 的 Stb 包解析得到
# 注意：第三方大多数头文件路径会通过 imported target 自动传播，不需要手工 include_directories。
target_include_directories(OpenGLToy
    PRIVATE
        "${PROJECT_SOURCE_DIR}/source"
        "${OPENGLTOY_STB_INCLUDE_DIR}")

# 编译宏：
#   GLFW_INCLUDE_NONE          防止 GLFW 自动把系统 OpenGL 头提前 include 进来
#                              否则会与 glad 的头文件装载策略冲突。
#   Alice_PROJECT_ROOT / ASSET_ROOT
#                              给运行期资源查找提供工程根与资源根。
target_compile_definitions(OpenGLToy
    PRIVATE
        GLFW_INCLUDE_NONE
        Alice_PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
        Alice_ASSET_ROOT="${OPENGLTOY_ASSET_ROOT}"
        $<$<CONFIG:Debug>:_DEBUG>)

# 链接 imported targets：
#   - OpenGL::GL         平台 OpenGL 入口
#   - GLFW / GLAD / GLM / FMT 由 vcpkg + find_package 解析而来
# 核心思想：
#   CMake 管的是“目标级依赖关系”，而不是单纯拼接 include/lib 路径。
#   目标被正确链接后，头文件路径、库路径、编译定义、传递依赖都会一并传播。
target_link_libraries(OpenGLToy
    PRIVATE
        OpenGL::GL
        ${OPENGLTOY_GLFW_TARGET}
        ${OPENGLTOY_GLAD_TARGET}
        glm::glm
        fmt::fmt)

# 平台化告警策略。
if(OPENGLTOY_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(OpenGLToy PRIVATE /W4 /permissive- /EHsc)
        if(OPENGLTOY_WARNINGS_AS_ERRORS)
            target_compile_options(OpenGLToy PRIVATE /WX)
        endif()
    else()
        target_compile_options(OpenGLToy PRIVATE -Wall -Wextra -Wpedantic)
        if(OPENGLTOY_WARNINGS_AS_ERRORS)
            target_compile_options(OpenGLToy PRIVATE -Werror)
        endif()
    endif()
endif()

install(TARGETS OpenGLToy RUNTIME DESTINATION bin)
