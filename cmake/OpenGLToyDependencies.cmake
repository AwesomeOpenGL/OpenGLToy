include_guard(GLOBAL)

# ==============================================================================
# OpenGLToyDependencies.cmake
#
# 目标：
#   1. 把第三方依赖解析逻辑从顶层 CMakeLists 分离出去
#   2. 对不同包可能暴露出来的 target 名称做兼容
#   3. 将最终可链接的 target 名称与必要的 include 目录回传给顶层工程
# ==============================================================================

function(opengltoy_pick_target out_var)
    foreach(candidate IN LISTS ARGN)
        if(TARGET "${candidate}")
            set(${out_var} "${candidate}" PARENT_SCOPE)
            return()
        endif()
    endforeach()

    string(REPLACE ";" ", " readable_candidates "${ARGN}")
    message(FATAL_ERROR "Unable to resolve dependency target. Checked: ${readable_candidates}")
endfunction()

function(opengltoy_resolve_dependencies)
    # --------------------------------------------------------------------------
    # find_package(...) 在 vcpkg toolchain 生效后，会优先去 vcpkg 管理的安装前缀中查找。
    # 这也是 manifest mode 的核心：
    #   - vcpkg 负责准备包
    #   - CMake 负责解析包并把它们组织为 imported targets
    # --------------------------------------------------------------------------
    find_package(OpenGL REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(Stb REQUIRED)

    if(NOT DEFINED Stb_INCLUDE_DIR OR Stb_INCLUDE_DIR STREQUAL "")
        message(FATAL_ERROR "Stb was found, but Stb_INCLUDE_DIR is empty.")
    endif()

    # 不同平台 / 不同 port / 不同配置脚本，有时导出的 target 名称不完全一致。
    # 这里做一个小兼容层，把“可用候选 target”统一收敛成工程内部变量。
    opengltoy_pick_target(OPENGLTOY_GLFW_TARGET glfw glfw3 glfw3::glfw glfw3::glfw3)
    opengltoy_pick_target(OPENGLTOY_GLAD_TARGET glad::glad glad)

    set(OPENGLTOY_GLFW_TARGET "${OPENGLTOY_GLFW_TARGET}" PARENT_SCOPE)
    set(OPENGLTOY_GLAD_TARGET "${OPENGLTOY_GLAD_TARGET}" PARENT_SCOPE)
    set(OPENGLTOY_STB_INCLUDE_DIR "${Stb_INCLUDE_DIR}" PARENT_SCOPE)
endfunction()
