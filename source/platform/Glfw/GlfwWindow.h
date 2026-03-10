#pragma once

#include "foundation/Base/NonCopyable.h"
#include "platform/Glfw/GlfwInputService.h"
#include "platform/IWindow.h"

struct GLFWwindow;

namespace Alice::Platform::Glfw
{
    class GlfwWindow final
        : public IWindow
        , private Foundation::Base::NonCopyable
    {
    public:
        GlfwWindow();
        ~GlfwWindow() override;

        bool Create(const WindowDesc& desc) override;
        void Destroy() override;
        void PollEvents() override;
        void SwapBuffers() override;
        bool ShouldClose() const override;
        void RequestClose() override;
        int GetWidth() const override;
        int GetHeight() const override;
        float GetAspectRatio() const override;
        void SetCursorMode(CursorMode mode) override;
        void SetVSyncEnabled(bool enabled) override;
        void* GetNativeHandle() const override;
        IInputService& GetInputService() override;
        const IInputService& GetInputService() const override;

    private:
        static void OnFramebufferSizeChanged(GLFWwindow* window, int width, int height);
        static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void OnCursorPosition(GLFWwindow* window, double x, double y);
        static void OnScroll(GLFWwindow* window, double xOffset, double yOffset);

        GLFWwindow* m_window = nullptr;
        GlfwInputService m_inputService;
        int m_width = 0;
        int m_height = 0;
    };
}
