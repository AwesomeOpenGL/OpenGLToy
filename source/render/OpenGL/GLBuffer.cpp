#include "render/OpenGL/GLBuffer.h"

namespace Alice::Render::OpenGL
{
    GLBuffer::GLBuffer(const GLenum target)
        : m_target(target)
    {
        glGenBuffers(1, &m_handle);
    }

    GLBuffer::~GLBuffer()
    {
        Release();
    }

    GLBuffer::GLBuffer(GLBuffer&& other) noexcept
        : m_target(other.m_target)
        , m_handle(other.m_handle)
    {
        other.m_handle = 0;
    }

    GLBuffer& GLBuffer::operator=(GLBuffer&& other) noexcept
    {
        if (this != &other)
        {
            Release();
            m_target = other.m_target;
            m_handle = other.m_handle;
            other.m_handle = 0;
        }
        return *this;
    }

    void GLBuffer::Bind() const
    {
        glBindBuffer(m_target, m_handle);
    }

    void GLBuffer::Unbind() const
    {
        glBindBuffer(m_target, 0);
    }

    void GLBuffer::SetData(const void* data, const std::size_t sizeInBytes, const GLenum usage) const
    {
        Bind();
        glBufferData(m_target, static_cast<GLsizeiptr>(sizeInBytes), data, usage);
    }

    GLuint GLBuffer::GetHandle() const
    {
        return m_handle;
    }

    void GLBuffer::Release()
    {
        if (m_handle != 0)
        {
            glDeleteBuffers(1, &m_handle);
            m_handle = 0;
        }
    }
}
