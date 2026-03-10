#include "render/Scene/Camera.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Alice::Render::Scene
{
    Camera::Camera()
    {
        UpdateCameraVectors();
    }

    Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch)
        : m_position(position)
        , m_worldUp(up)
        , m_yawDegrees(yaw)
        , m_pitchDegrees(pitch)
    {
        UpdateCameraVectors();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::ProcessKeyboard(const CameraMovement direction, const float deltaSeconds)
    {
        const float velocity = m_movementSpeed * deltaSeconds;

        if (direction == CameraMovement::Forward)
        {
            m_position += m_front * velocity;
        }
        else if (direction == CameraMovement::Backward)
        {
            m_position -= m_front * velocity;
        }
        else if (direction == CameraMovement::Left)
        {
            m_position -= m_right * velocity;
        }
        else if (direction == CameraMovement::Right)
        {
            m_position += m_right * velocity;
        }
    }

    void Camera::ProcessMouseMovement(float xOffset, float yOffset, const bool constrainPitch)
    {
        xOffset *= m_mouseSensitivity;
        yOffset *= m_mouseSensitivity;

        m_yawDegrees += xOffset;
        m_pitchDegrees += yOffset;

        if (constrainPitch)
        {
            if (m_pitchDegrees > 89.0f)
            {
                m_pitchDegrees = 89.0f;
            }

            if (m_pitchDegrees < -89.0f)
            {
                m_pitchDegrees = -89.0f;
            }
        }

        UpdateCameraVectors();
    }

    void Camera::ProcessMouseScroll(const float yOffset)
    {
        m_zoomDegrees -= yOffset;

        if (m_zoomDegrees < 1.0f)
        {
            m_zoomDegrees = 1.0f;
        }

        if (m_zoomDegrees > 45.0f)
        {
            m_zoomDegrees = 45.0f;
        }
    }

    const glm::vec3& Camera::GetPosition() const
    {
        return m_position;
    }

    float Camera::GetZoomDegrees() const
    {
        return m_zoomDegrees;
    }

    void Camera::UpdateCameraVectors()
    {
        glm::vec3 front{};
        front.x = glm::cos(glm::radians(m_yawDegrees)) * glm::cos(glm::radians(m_pitchDegrees));
        front.y = glm::sin(glm::radians(m_pitchDegrees));
        front.z = glm::sin(glm::radians(m_yawDegrees)) * glm::cos(glm::radians(m_pitchDegrees));

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
}
