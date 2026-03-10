#include "render/OpenGL/GLTexture2D.h"

#include "foundation/Diagnostics/Log.h"

#include <fmt/core.h>
#include <stb_image.h>

namespace Alice::Render::OpenGL
{
    GLTexture2D::~GLTexture2D()
    {
        Destroy();
    }

    GLTexture2D::GLTexture2D(GLTexture2D&& other) noexcept
        : m_handle(other.m_handle)
        , m_width(other.m_width)
        , m_height(other.m_height)
    {
        other.m_handle = 0;
        other.m_width = 0;
        other.m_height = 0;
    }

    GLTexture2D& GLTexture2D::operator=(GLTexture2D&& other) noexcept
    {
        if (this != &other)
        {
            Destroy();
            m_handle = other.m_handle;
            m_width = other.m_width;
            m_height = other.m_height;
            other.m_handle = 0;
            other.m_width = 0;
            other.m_height = 0;
        }

        return *this;
    }

    bool GLTexture2D::CreateFromFile(const std::string& filePath, const bool flipVertically)
    {
        Destroy();

        stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

        int channels = 0;
        unsigned char* pixels = stbi_load(filePath.c_str(), &m_width, &m_height, &channels, 0);
        if (pixels == nullptr)
        {
            LEOCPP_LOG_ERROR(fmt::format("Failed to load texture: {}", filePath));
            return false;
        }

        GLenum format = GL_RGB;
        if (channels == 1)
        {
            format = GL_RED;
        }
        else if (channels == 3)
        {
            format = GL_RGB;
        }
        else if (channels == 4)
        {
            format = GL_RGBA;
        }

        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(pixels);
        return true;
    }

    void GLTexture2D::Destroy()
    {
        if (m_handle != 0)
        {
            glDeleteTextures(1, &m_handle);
            m_handle = 0;
            m_width = 0;
            m_height = 0;
        }
    }

    void GLTexture2D::Bind(const unsigned int textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }

    GLuint GLTexture2D::GetHandle() const
    {
        return m_handle;
    }

    int GLTexture2D::GetWidth() const
    {
        return m_width;
    }

    int GLTexture2D::GetHeight() const
    {
        return m_height;
    }
}
