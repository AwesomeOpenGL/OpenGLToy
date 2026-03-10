#include "samples/getting_started/SampleTransformations.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
    constexpr float GVertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    constexpr unsigned int GIndices[] = {
        0, 1, 3,
        1, 2, 3
    };
}

namespace Alice::Samples::GettingStarted
{
    bool SampleTransformations::OnInitialize(const SampleInitializationContext& context)
    {
        m_assetRoot = context.AssetRoot;
        if (!m_shaderProgram.CreateFromFiles(
                m_assetRoot + "/shaders/getting_started/transformations.vert",
                m_assetRoot + "/shaders/getting_started/transformations.frag"))
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
        m_vertexBuffer.SetData(GVertices, sizeof(GVertices), GL_STATIC_DRAW);
        m_indexBuffer.SetData(GIndices, sizeof(GIndices), GL_STATIC_DRAW);

        constexpr GLsizei stride = 8 * static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        m_shaderProgram.Use();
        m_shaderProgram.SetInt("uTexture0", 0);
        m_shaderProgram.SetInt("uTexture1", 1);

        Render::OpenGL::GLVertexArray::Unbind();
        return true;
    }

    void SampleTransformations::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(0.35f, -0.25f, 0.0f));
        transform = glm::rotate(transform, static_cast<float>(context.ElapsedSeconds), glm::vec3(0.0f, 0.0f, 1.0f));

        m_baseTexture.Bind(0);
        m_overlayTexture.Bind(1);

        m_shaderProgram.Use();
        m_shaderProgram.SetMat4("uTransform", transform);

        m_vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void SampleTransformations::OnShutdown()
    {
    }
}
