#include "platform/Glfw/GlfwWindow.h"

#include "foundation/Diagnostics/Assert.h"
#include "foundation/Diagnostics/Log.h"
#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

namespace
{
    bool GIsGlfwInitialized = false;

    void EnsureGlfwInitialized()
    {
        if (GIsGlfwInitialized)
        {
            return;
        }

        if (glfwInit() == GLFW_FALSE)
        {
            LEOCPP_ASSERT(false, "glfwInit failed.");
        }

        GIsGlfwInitialized = true;
    }
}

namespace Alice::Platform::Glfw
{
    GlfwWindow::GlfwWindow() = default;

    GlfwWindow::~GlfwWindow()
    {
        Destroy();
    }

    bool GlfwWindow::Create(const WindowDesc& desc)
    {
        EnsureGlfwInitialized();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc.OpenGLMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc.OpenGLMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, desc.Resizable ? GLFW_TRUE : GLFW_FALSE);

    #if defined(_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, desc.EnableOpenGLDebugContext ? GLFW_TRUE : GLFW_FALSE);
    #endif
    #if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif

        m_window = glfwCreateWindow(desc.Width, desc.Height, desc.Title.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            LEOCPP_LOG_ERROR("glfwCreateWindow failed.");
            return false;
        }

        m_width = desc.Width;
        m_height = desc.Height;

        glfwSetWindowUserPointer(m_window, this);
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, &GlfwWindow::OnFramebufferSizeChanged);
        glfwSetKeyCallback(m_window, &GlfwWindow::OnKey);
        glfwSetCursorPosCallback(m_window, &GlfwWindow::OnCursorPosition);
        glfwSetScrollCallback(m_window, &GlfwWindow::OnScroll);

        SetVSyncEnabled(desc.VSync);
        return true;
    }

    void GlfwWindow::Destroy()
    {
        if (m_window != nullptr)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void GlfwWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GlfwWindow::SwapBuffers()
    {
        LEOCPP_ASSERT(m_window != nullptr, "SwapBuffers requires a valid window.");
        glfwSwapBuffers(m_window);
    }

    bool GlfwWindow::ShouldClose() const
    {
        return m_window == nullptr || glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    void GlfwWindow::RequestClose()
    {
        if (m_window != nullptr)
        {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }
    }

    int GlfwWindow::GetWidth() const
    {
        return m_width;
    }

    int GlfwWindow::GetHeight() const
    {
        return m_height;
    }

    float GlfwWindow::GetAspectRatio() const
    {
        return m_height > 0 ? static_cast<float>(m_width) / static_cast<float>(m_height) : 1.0f;
    }

    void GlfwWindow::SetCursorMode(const CursorMode mode)
    {
        LEOCPP_ASSERT(m_window != nullptr, "SetCursorMode requires a valid window.");

        const int glfwMode = (mode == CursorMode::Disabled) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
        glfwSetInputMode(m_window, GLFW_CURSOR, glfwMode);
    }

    void GlfwWindow::SetVSyncEnabled(const bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void* GlfwWindow::GetNativeHandle() const
    {
        return m_window;
    }

    IInputService& GlfwWindow::GetInputService()
    {
        return m_inputService;
    }

    const IInputService& GlfwWindow::GetInputService() const
    {
        return m_inputService;
    }

    void GlfwWindow::OnFramebufferSizeChanged(GLFWwindow* window, const int width, const int height)
    {
        auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self == nullptr)
        {
            return;
        }

        self->m_width = width;
        self->m_height = height;
        glViewport(0, 0, width, height);
    }

    void GlfwWindow::OnKey(GLFWwindow* window, const int key, const int, const int action, const int)
    {
        auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self == nullptr)
        {
            return;
        }

        self->m_inputService.OnKeyChanged(key, action);
    }

    void GlfwWindow::OnCursorPosition(GLFWwindow* window, const double x, const double y)
    {
        auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self == nullptr)
        {
            return;
        }

        self->m_inputService.OnCursorMoved(x, y);
    }

    void GlfwWindow::OnScroll(GLFWwindow* window, const double xOffset, const double yOffset)
    {
        auto* self = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
        if (self == nullptr)
        {
            return;
        }

        self->m_inputService.OnScroll(xOffset, yOffset);
    }
}
