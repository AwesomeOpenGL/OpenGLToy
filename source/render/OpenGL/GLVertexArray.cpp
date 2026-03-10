#include "render/OpenGL/GLVertexArray.h"

namespace Alice::Render::OpenGL
{
    GLVertexArray::GLVertexArray()
    {
        glGenVertexArrays(1, &m_handle);
    }

    GLVertexArray::~GLVertexArray()
    {
        Release();
    }

    GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = 0;
    }

    GLVertexArray& GLVertexArray::operator=(GLVertexArray&& other) noexcept
    {
        if (this != &other)
        {
            Release();
            m_handle = other.m_handle;
            other.m_handle = 0;
        }
        return *this;
    }

    void GLVertexArray::Bind() const
    {
        glBindVertexArray(m_handle);
    }

    void GLVertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    GLuint GLVertexArray::GetHandle() const
    {
        return m_handle;
    }

    void GLVertexArray::Release()
    {
        if (m_handle != 0)
        {
            glDeleteVertexArrays(1, &m_handle);
            m_handle = 0;
        }
    }
}
