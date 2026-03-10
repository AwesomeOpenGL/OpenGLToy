#include "platform/Glfw/GlfwInputService.h"

namespace Alice::Platform::Glfw
{
    GlfwInputService::GlfwInputService() = default;

    void GlfwInputService::BeginFrame()
    {
        m_previousKeyStates = m_currentKeyStates;
        m_mouseDeltaX = 0.0;
        m_mouseDeltaY = 0.0;
    }

    bool GlfwInputService::IsKeyDown(const int key) const
    {
        if (key < 0 || key >= MaxTrackedKeys)
        {
            return false;
        }

        return m_currentKeyStates[static_cast<std::size_t>(key)];
    }

    bool GlfwInputService::WasKeyPressed(const int key) const
    {
        if (key < 0 || key >= MaxTrackedKeys)
        {
            return false;
        }

        const std::size_t index = static_cast<std::size_t>(key);
        return m_currentKeyStates[index] && !m_previousKeyStates[index];
    }

    bool GlfwInputService::WasKeyReleased(const int key) const
    {
        if (key < 0 || key >= MaxTrackedKeys)
        {
            return false;
        }

        const std::size_t index = static_cast<std::size_t>(key);
        return !m_currentKeyStates[index] && m_previousKeyStates[index];
    }

    double GlfwInputService::GetMouseX() const
    {
        return m_mouseX;
    }

    double GlfwInputService::GetMouseY() const
    {
        return m_mouseY;
    }

    double GlfwInputService::GetMouseDeltaX() const
    {
        return m_mouseDeltaX;
    }

    double GlfwInputService::GetMouseDeltaY() const
    {
        return m_mouseDeltaY;
    }

    double GlfwInputService::ConsumeScrollDeltaY()
    {
        const double result = m_scrollDeltaY;
        m_scrollDeltaY = 0.0;
        return result;
    }

    void GlfwInputService::OnKeyChanged(const int key, const int action)
    {
        if (key < 0 || key >= MaxTrackedKeys)
        {
            return;
        }

        m_currentKeyStates[static_cast<std::size_t>(key)] = (action != 0);
    }

    void GlfwInputService::OnCursorMoved(const double x, const double y)
    {
        if (!m_hasMousePosition)
        {
            m_previousMouseX = x;
            m_previousMouseY = y;
            m_hasMousePosition = true;
        }

        m_mouseX = x;
        m_mouseY = y;
        m_mouseDeltaX += x - m_previousMouseX;
        m_mouseDeltaY += m_previousMouseY - y;
        m_previousMouseX = x;
        m_previousMouseY = y;
    }

    void GlfwInputService::OnScroll(const double, const double yOffset)
    {
        m_scrollDeltaY += yOffset;
    }
}
