#include "render/OpenGL/GLShaderProgram.h"

#include "foundation/Diagnostics/Log.h"
#include "foundation/IO/FileSystem.h"

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>

namespace Alice::Render::OpenGL
{
    GLShaderProgram::~GLShaderProgram()
    {
        Destroy();
    }

    GLShaderProgram::GLShaderProgram(GLShaderProgram&& other) noexcept
        : m_programHandle(other.m_programHandle)
    {
        other.m_programHandle = 0;
    }

    GLShaderProgram& GLShaderProgram::operator=(GLShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            Destroy();
            m_programHandle = other.m_programHandle;
            other.m_programHandle = 0;
        }
        return *this;
    }

    bool GLShaderProgram::CreateFromSource(const std::string& vertexSource, const std::string& fragmentSource)
    {
        Destroy();

        GLuint vertexShader = 0;
        if (!CompileShader(GL_VERTEX_SHADER, vertexSource, vertexShader))
        {
            return false;
        }

        GLuint fragmentShader = 0;
        if (!CompileShader(GL_FRAGMENT_SHADER, fragmentSource, fragmentShader))
        {
            glDeleteShader(vertexShader);
            return false;
        }

        m_programHandle = glCreateProgram();
        glAttachShader(m_programHandle, vertexShader);
        glAttachShader(m_programHandle, fragmentShader);
        glLinkProgram(m_programHandle);

        GLint didLink = GL_FALSE;
        glGetProgramiv(m_programHandle, GL_LINK_STATUS, &didLink);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (didLink == GL_FALSE)
        {
            LEOCPP_LOG_ERROR(fmt::format("Program link failed:\n{}", ReadInfoLog(m_programHandle, true)));
            Destroy();
            return false;
        }

        return true;
    }

    bool GLShaderProgram::CreateFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
    {
        const std::string vertexSource = Foundation::IO::ReadAllText(vertexPath);
        const std::string fragmentSource = Foundation::IO::ReadAllText(fragmentPath);
        return CreateFromSource(vertexSource, fragmentSource);
    }

    void GLShaderProgram::Destroy()
    {
        if (m_programHandle != 0)
        {
            glDeleteProgram(m_programHandle);
            m_programHandle = 0;
        }
    }

    void GLShaderProgram::Use() const
    {
        glUseProgram(m_programHandle);
    }

    GLuint GLShaderProgram::GetHandle() const
    {
        return m_programHandle;
    }

    void GLShaderProgram::SetBool(const char* name, const bool value) const
    {
        glUniform1i(GetUniformLocation(name), value ? 1 : 0);
    }

    void GLShaderProgram::SetInt(const char* name, const int value) const
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void GLShaderProgram::SetFloat(const char* name, const float value) const
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void GLShaderProgram::SetVec3(const char* name, const glm::vec3& value) const
    {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
    }

    void GLShaderProgram::SetMat4(const char* name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    bool GLShaderProgram::CompileShader(const GLenum shaderType, const std::string& source, GLuint& outShaderHandle)
    {
        outShaderHandle = glCreateShader(shaderType);

        const char* sourcePtr = source.c_str();
        glShaderSource(outShaderHandle, 1, &sourcePtr, nullptr);
        glCompileShader(outShaderHandle);

        GLint didCompile = GL_FALSE;
        glGetShaderiv(outShaderHandle, GL_COMPILE_STATUS, &didCompile);
        if (didCompile == GL_FALSE)
        {
            LEOCPP_LOG_ERROR(fmt::format("Shader compilation failed:\n{}", ReadInfoLog(outShaderHandle, false)));
            glDeleteShader(outShaderHandle);
            outShaderHandle = 0;
            return false;
        }

        return true;
    }

    std::string GLShaderProgram::ReadInfoLog(const GLuint handle, const bool isProgram)
    {
        GLint infoLogLength = 0;
        if (isProgram)
        {
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
        }
        else
        {
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
        }

        if (infoLogLength <= 1)
        {
            return {};
        }

        std::string infoLog(static_cast<std::size_t>(infoLogLength), '\0');
        if (isProgram)
        {
            glGetProgramInfoLog(handle, infoLogLength, nullptr, infoLog.data());
        }
        else
        {
            glGetShaderInfoLog(handle, infoLogLength, nullptr, infoLog.data());
        }

        return infoLog;
    }

    GLint GLShaderProgram::GetUniformLocation(const char* name) const
    {
        return glGetUniformLocation(m_programHandle, name);
    }
}
