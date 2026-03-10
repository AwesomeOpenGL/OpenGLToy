#include "samples/getting_started/SampleHelloWindow.h"

#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

namespace Alice::Samples::GettingStarted
{
    bool SampleHelloWindow::OnInitialize(const SampleInitializationContext&)
    {
        return true;
    }

    void SampleHelloWindow::OnUpdate(const SampleFrameContext& context)
    {
        if (context.Input.IsKeyDown(GLFW_KEY_ESCAPE))
        {
            context.Window.RequestClose();
        }

        glViewport(0, 0, context.Window.GetWidth(), context.Window.GetHeight());
        glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void SampleHelloWindow::OnShutdown()
    {
    }
}
