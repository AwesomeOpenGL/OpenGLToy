#pragma once

#include "render/OpenGL/GLBuffer.h"
#include "render/OpenGL/GLShaderProgram.h"
#include "render/OpenGL/GLVertexArray.h"
#include "samples/ISample.h"

#include <string>

namespace Alice::Samples::GettingStarted
{
    class SampleShadersBasic final : public ISample
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
    };
}
