#pragma once

#include <string>

namespace Alice::Platform
{
    enum class CursorMode
    {
        Normal,
        Disabled
    };

    struct WindowDesc
    {
        int Width = 1280;
        int Height = 720;
        std::string Title = "Alice";
        int OpenGLMajorVersion = 3;
        int OpenGLMinorVersion = 3;
        bool EnableOpenGLDebugContext = true;
        bool Resizable = true;
        bool VSync = true;
    };
}
