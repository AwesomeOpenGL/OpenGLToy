#include "samples/getting_started/SampleShadersBasic.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

namespace
{
    constexpr float GVertices[] = {
         0.5f, -0.5f, 0.0f,  1.0f, 0.1f, 0.1f,
        -0.5f, -0.5f, 0.0f,  0.1f, 1.0f, 0.1f,
         0.0f,  0.5f, 0.0f,  0.1f, 0.2f, 1.0f
    };
}

namespace Alice::Samples::GettingStarted
{
    bool SampleShadersBasic::OnInitialize(const SampleInitializationContext& context)
    {
        m_assetRoot = context.AssetRoot;
        const std::string vertexPath = m_assetRoot + "/shaders/getting_started/shaders_basic.vert";
        const std::string fragmentPath = m_assetRoot + "/shaders/getting_started/shaders_basic.frag";

        if (!m_shaderProgram.CreateFromFiles(vertexPath, fragmentPath))
        {
            return false;
        }

        m_vertexArray.Bind();
        m_vertexBuffer.SetData(GVertices, sizeof(GVertices), GL_STATIC_DRAW);

        constexpr GLsizei stride = 6 * static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        Render::OpenGL::GLVertexArray::Unbind();
        return true;
    }

    void SampleShadersBasic::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shaderProgram.Use();
        m_vertexArray.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void SampleShadersBasic::OnShutdown()
    {
    }
}
