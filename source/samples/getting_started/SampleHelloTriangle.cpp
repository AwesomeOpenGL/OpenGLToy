#include "samples/getting_started/SampleHelloTriangle.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

namespace
{
    constexpr float GTriangleVertices[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    constexpr const char* GVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPosition;
        void main()
        {
            gl_Position = vec4(aPosition, 1.0);
        }
    )";

    constexpr const char* GFragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0, 0.45, 0.2, 1.0);
        }
    )";
}

namespace Alice::Samples::GettingStarted
{
    bool SampleHelloTriangle::OnInitialize(const SampleInitializationContext&)
    {
        if (!m_shaderProgram.CreateFromSource(GVertexShaderSource, GFragmentShaderSource))
        {
            return false;
        }

        m_vertexArray.Bind();
        m_vertexBuffer.SetData(GTriangleVertices, sizeof(GTriangleVertices), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * static_cast<GLsizei>(sizeof(float)), nullptr);
        glEnableVertexAttribArray(0);

        Render::OpenGL::GLVertexArray::Unbind();
        return true;
    }

    void SampleHelloTriangle::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shaderProgram.Use();
        m_vertexArray.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void SampleHelloTriangle::OnShutdown()
    {
    }
}
