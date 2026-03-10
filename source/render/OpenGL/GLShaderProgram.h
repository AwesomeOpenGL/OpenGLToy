#pragma once

#include "foundation/Base/NonCopyable.h"
#include "render/OpenGL/GLCommon.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <string>

namespace Alice::Render::OpenGL
{
    class GLShaderProgram final : private Foundation::Base::NonCopyable
    {
    public:
        GLShaderProgram() = default;
        ~GLShaderProgram();

        GLShaderProgram(GLShaderProgram&& other) noexcept;
        GLShaderProgram& operator=(GLShaderProgram&& other) noexcept;

        bool CreateFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        bool CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
        void Destroy();

        void Use() const;
        GLuint GetHandle() const;

        void SetBool(const char* name, bool value) const;
        void SetInt(const char* name, int value) const;
        void SetFloat(const char* name, float value) const;
        void SetVec3(const char* name, const glm::vec3& value) const;
        void SetMat4(const char* name, const glm::mat4& value) const;

    private:
        static bool CompileShader(GLenum shaderType, const std::string& source, GLuint& outShaderHandle);
        static std::string ReadInfoLog(GLuint handle, bool isProgram);

        GLint GetUniformLocation(const char* name) const;

        GLuint m_programHandle = 0;
    };
}
