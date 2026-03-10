#include "samples/getting_started/SampleCameraClass.h"

#include "platform/WindowTypes.h"
#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iterator>

namespace
{
    constexpr float GCubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    const glm::vec3 GCubePositions[] = {
        { 0.0f,  0.0f,  0.0f },
        { 2.0f,  5.0f, -15.0f },
        { -1.5f, -2.2f, -2.5f },
        { -3.8f, -2.0f, -12.3f },
        { 2.4f, -0.4f, -3.5f },
        { -1.7f,  3.0f, -7.5f },
        { 1.3f, -2.0f, -2.5f },
        { 1.5f,  2.0f, -2.5f },
        { 1.5f,  0.2f, -1.5f },
        { -1.3f,  1.0f, -1.5f }
    };
}

namespace Alice::Samples::GettingStarted
{
    bool SampleCameraClass::OnInitialize(const SampleInitializationContext& context)
    {
        m_assetRoot = context.AssetRoot;

        if (!m_shaderProgram.CreateFromFiles(
                m_assetRoot + "/shaders/getting_started/camera_class.vert",
                m_assetRoot + "/shaders/getting_started/camera_class.frag"))
        {
            return false;
        }

        if (!m_baseTexture.CreateFromFile(m_assetRoot + "/textures/crate_diffuse.png"))
        {
            return false;
        }

        if (!m_overlayTexture.CreateFromFile(m_assetRoot + "/textures/awesomeface.png"))
        {
            return false;
        }

        m_vertexArray.Bind();
        m_vertexBuffer.SetData(GCubeVertices, sizeof(GCubeVertices), GL_STATIC_DRAW);

        constexpr GLsizei stride = 5 * static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_shaderProgram.Use();
        m_shaderProgram.SetInt("uTexture0", 0);
        m_shaderProgram.SetInt("uTexture1", 1);

        glEnable(GL_DEPTH_TEST);
        context.Window.SetCursorMode(Platform::CursorMode::Disabled);
        return true;
    }

    void SampleCameraClass::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        if (context.Input.IsKeyDown(GLFW_KEY_W))
        {
            m_camera.ProcessKeyboard(Render::Scene::CameraMovement::Forward, context.DeltaSeconds);
        }

        if (context.Input.IsKeyDown(GLFW_KEY_S))
        {
            m_camera.ProcessKeyboard(Render::Scene::CameraMovement::Backward, context.DeltaSeconds);
        }

        if (context.Input.IsKeyDown(GLFW_KEY_A))
        {
            m_camera.ProcessKeyboard(Render::Scene::CameraMovement::Left, context.DeltaSeconds);
        }

        if (context.Input.IsKeyDown(GLFW_KEY_D))
        {
            m_camera.ProcessKeyboard(Render::Scene::CameraMovement::Right, context.DeltaSeconds);
        }

        m_camera.ProcessMouseMovement(
            static_cast<float>(context.Input.GetMouseDeltaX()),
            static_cast<float>(context.Input.GetMouseDeltaY()));

        m_camera.ProcessMouseScroll(static_cast<float>(context.Input.ConsumeScrollDeltaY()));

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.04f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = m_camera.GetViewMatrix();
        const glm::mat4 projection = glm::perspective(
            glm::radians(m_camera.GetZoomDegrees()),
            context.Window.GetAspectRatio(),
            0.1f,
            100.0f);

        m_baseTexture.Bind(0);
        m_overlayTexture.Bind(1);

        m_shaderProgram.Use();
        m_shaderProgram.SetMat4("uView", view);
        m_shaderProgram.SetMat4("uProjection", projection);

        m_vertexArray.Bind();

        for (unsigned int index = 0; index < static_cast<unsigned int>(std::size(GCubePositions)); ++index)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, GCubePositions[index]);

            const float angle = 20.0f * static_cast<float>(index);
            model = glm::rotate(
                model,
                glm::radians(angle) + static_cast<float>(context.ElapsedSeconds * 0.25),
                glm::vec3(1.0f, 0.3f, 0.5f));

            m_shaderProgram.SetMat4("uModel", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void SampleCameraClass::OnShutdown()
    {
        glDisable(GL_DEPTH_TEST);
    }
}
