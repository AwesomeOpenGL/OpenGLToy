#pragma once

#include "foundation/Base/NonCopyable.h"
#include "render/OpenGL/GLCommon.h"

namespace Alice::Render::OpenGL
{
    class GLVertexArray final : private Foundation::Base::NonCopyable
    {
    public:
        GLVertexArray();
        ~GLVertexArray();

        GLVertexArray(GLVertexArray&& other) noexcept;
        GLVertexArray& operator=(GLVertexArray&& other) noexcept;

        void Bind() const;
        static void Unbind();
        GLuint GetHandle() const;

    private:
        void Release();

        GLuint m_handle = 0;
    };
}
