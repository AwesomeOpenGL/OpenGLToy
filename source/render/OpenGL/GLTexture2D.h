#pragma once

#include "foundation/Base/NonCopyable.h"
#include "render/OpenGL/GLCommon.h"

#include <string>

namespace Alice::Render::OpenGL
{
    class GLTexture2D final : private Foundation::Base::NonCopyable
    {
    public:
        GLTexture2D() = default;
        ~GLTexture2D();

        GLTexture2D(GLTexture2D&& other) noexcept;
        GLTexture2D& operator=(GLTexture2D&& other) noexcept;

        bool CreateFromFile(const std::string& filePath, bool flipVertically = true);
        void Destroy();

        void Bind(unsigned int textureUnit) const;
        GLuint GetHandle() const;
        int GetWidth() const;
        int GetHeight() const;

    private:
        GLuint m_handle = 0;
        int m_width = 0;
        int m_height = 0;
    };
}
