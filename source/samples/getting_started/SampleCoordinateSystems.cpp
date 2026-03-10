#include "samples/getting_started/SampleCoordinateSystems.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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
}

namespace Alice::Samples::GettingStarted
{
    bool SampleCoordinateSystems::OnInitialize(const SampleInitializationContext& context)
    {
        m_assetRoot = context.AssetRoot;
        if (!m_shaderProgram.CreateFromFiles(
                m_assetRoot + "/shaders/getting_started/coordinate_systems.vert",
                m_assetRoot + "/shaders/getting_started/coordinate_systems.frag"))
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
        Render::OpenGL::GLVertexArray::Unbind();
        return true;
    }

    void SampleCoordinateSystems::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.06f, 0.07f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        const glm::mat4 projection = glm::perspective(glm::radians(45.0f), context.Window.GetAspectRatio(), 0.1f, 100.0f);

        m_baseTexture.Bind(0);
        m_overlayTexture.Bind(1);

        m_shaderProgram.Use();
        m_shaderProgram.SetMat4("uModel", model);
        m_shaderProgram.SetMat4("uView", view);
        m_shaderProgram.SetMat4("uProjection", projection);

        m_vertexArray.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void SampleCoordinateSystems::OnShutdown()
    {
        glDisable(GL_DEPTH_TEST);
    }
}
