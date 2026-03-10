#pragma once

#include "foundation/Base/NonCopyable.h"
#include "render/OpenGL/GLCommon.h"

#include <cstddef>

namespace Alice::Render::OpenGL
{
    class GLBuffer final : private Foundation::Base::NonCopyable
    {
    public:
        explicit GLBuffer(GLenum target);
        ~GLBuffer();

        GLBuffer(GLBuffer&& other) noexcept;
        GLBuffer& operator=(GLBuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;
        void SetData(const void* data, std::size_t sizeInBytes, GLenum usage) const;
        GLuint GetHandle() const;

    private:
        void Release();

        GLenum m_target = GL_ARRAY_BUFFER;
        GLuint m_handle = 0;
    };
}
