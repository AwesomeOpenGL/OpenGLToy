#include "samples/getting_started/SampleTexturesBasic.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

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
    bool SampleTexturesBasic::OnInitialize(const SampleInitializationContext& context)
    {
        m_assetRoot = context.AssetRoot;
        const std::string vertexPath = m_assetRoot + "/shaders/getting_started/textures_basic.vert";
        const std::string fragmentPath = m_assetRoot + "/shaders/getting_started/textures_basic.frag";

        if (!m_shaderProgram.CreateFromFiles(vertexPath, fragmentPath))
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

    void SampleTexturesBasic::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.10f, 0.11f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_baseTexture.Bind(0);
        m_overlayTexture.Bind(1);

        m_shaderProgram.Use();
        m_shaderProgram.SetFloat("uMixValue", 0.20f);
        m_vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void SampleTexturesBasic::OnShutdown()
    {
    }
}
