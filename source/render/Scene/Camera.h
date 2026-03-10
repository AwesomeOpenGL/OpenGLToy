#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Alice::Render::Scene
{
    enum class CameraMovement
    {
        Forward,
        Backward,
        Left,
        Right
    };

    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

        glm::mat4 GetViewMatrix() const;

        void ProcessKeyboard(CameraMovement direction, float deltaSeconds);
        void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yOffset);

        const glm::vec3& GetPosition() const;
        float GetZoomDegrees() const;

    private:
        void UpdateCameraVectors();

        glm::vec3 m_position{ 0.0f, 0.0f, 3.0f };
        glm::vec3 m_worldUp{ 0.0f, 1.0f, 0.0f };
        glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };

        float m_yawDegrees = -90.0f;
        float m_pitchDegrees = 0.0f;
        float m_movementSpeed = 2.5f;
        float m_mouseSensitivity = 0.1f;
        float m_zoomDegrees = 45.0f;
    };
}
