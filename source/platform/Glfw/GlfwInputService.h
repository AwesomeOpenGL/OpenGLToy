#pragma once

#include "foundation/Base/NonCopyable.h"
#include "platform/IInputService.h"

#include <array>

namespace Alice::Platform::Glfw
{
    class GlfwInputService final
        : public IInputService
        , private Foundation::Base::NonCopyable
    {
    public:
        GlfwInputService();

        void BeginFrame() override;
        bool IsKeyDown(int key) const override;
        bool WasKeyPressed(int key) const override;
        bool WasKeyReleased(int key) const override;
        double GetMouseX() const override;
        double GetMouseY() const override;
        double GetMouseDeltaX() const override;
        double GetMouseDeltaY() const override;
        double ConsumeScrollDeltaY() override;

        void OnKeyChanged(int key, int action);
        void OnCursorMoved(double x, double y);
        void OnScroll(double xOffset, double yOffset);

    private:
        static constexpr int MaxTrackedKeys = 512;

        std::array<bool, MaxTrackedKeys> m_currentKeyStates{};
        std::array<bool, MaxTrackedKeys> m_previousKeyStates{};

        double m_mouseX = 0.0;
        double m_mouseY = 0.0;
        double m_previousMouseX = 0.0;
        double m_previousMouseY = 0.0;
        double m_mouseDeltaX = 0.0;
        double m_mouseDeltaY = 0.0;
        double m_scrollDeltaY = 0.0;
        bool m_hasMousePosition = false;
    };
}
