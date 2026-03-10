#include "render/OpenGL/GLLoader.h"

#include "foundation/Diagnostics/Log.h"
#include "render/OpenGL/GLCommon.h"

#include <GLFW/glfw3.h>

namespace Alice::Render::OpenGL
{
    bool InitializeOpenGLLoader(void*)
    {
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        {
            LEOCPP_LOG_ERROR("Failed to initialize GLAD.");
            return false;
        }

        return true;
    }
}
