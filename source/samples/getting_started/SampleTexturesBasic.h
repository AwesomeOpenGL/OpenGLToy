#pragma once

#include "render/OpenGL/GLBuffer.h"
#include "render/OpenGL/GLShaderProgram.h"
#include "render/OpenGL/GLTexture2D.h"
#include "render/OpenGL/GLVertexArray.h"
#include "samples/ISample.h"

#include <string>

namespace Alice::Samples::GettingStarted
{
    class SampleTexturesBasic final : public ISample
    {
    public:
        bool OnInitialize(const SampleInitializationContext& context) override;
        void OnUpdate(const SampleFrameContext& context) override;
        void OnShutdown() override;

    private:
        std::string m_assetRoot;
        Render::OpenGL::GLShaderProgram m_shaderProgram;
        Render::OpenGL::GLVertexArray m_vertexArray;
        Render::OpenGL::GLBuffer m_vertexBuffer{ GL_ARRAY_BUFFER };
        Render::OpenGL::GLBuffer m_indexBuffer{ GL_ELEMENT_ARRAY_BUFFER };
        Render::OpenGL::GLTexture2D m_baseTexture;
        Render::OpenGL::GLTexture2D m_overlayTexture;
    };
}
