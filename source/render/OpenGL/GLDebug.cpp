#include "render/OpenGL/GLDebug.h"

#include "foundation/Diagnostics/Log.h"
#include "render/OpenGL/GLCommon.h"

#include <fmt/core.h>

namespace
{
#if defined(GL_VERSION_4_3) || defined(GL_KHR_debug)
    void APIENTRY OpenGLDebugCallback(GLenum, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            return;
        }

        Alice::Foundation::Diagnostics::WriteLog(
            Alice::Foundation::Diagnostics::LogLevel::Warning,
            fmt::format("OpenGL Debug [{}:{}] {}", static_cast<unsigned int>(type), static_cast<unsigned int>(severity), message));
    }
#endif
}

namespace Alice::Render::OpenGL
{
    void EnableOpenGLDebugOutput()
    {
#if defined(GL_VERSION_4_3) || defined(GL_KHR_debug)
        const bool hasDebugApi =
#if defined(GLAD_GL_KHR_debug)
            (GLAD_GL_KHR_debug != 0) ||
#endif
            (GLAD_GL_VERSION_4_3 != 0);

        if (!hasDebugApi)
        {
            LEOCPP_LOG_WARN("OpenGL debug output is not supported by the current driver/context.");
            return;
        }

        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0)
        {
            LEOCPP_LOG_WARN("OpenGL debug context is not available.");
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(&OpenGLDebugCallback, nullptr);
#else
        LEOCPP_LOG_WARN("OpenGL debug output was not compiled into GLAD. Enable gl-api-43 or KHR_debug support.");
#endif
    }
}
