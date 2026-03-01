#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace LGE
{
    class ShaderProgram
    {
    private:
        unsigned int m_Id; // Shader Program id
        std::unordered_map<std::string, int> m_UniformLocationCache;
    public:
        ShaderProgram(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc, const std::string& name = "unnamed");
        ~ShaderProgram();

        void Bind() const;
        void Unbind() const;

        void SetUniform1i(const std::string& name, int v);
        void SetUniform1f(const std::string& name, float v);
        void SetUniform2f(const std::string& name, float v0, float v1);
        void SetUniform3f(const std::string& name, float v0, float v1, float v2);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
    private:
        unsigned int CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc, const std::string& name);
        unsigned int CompileShader(unsigned int type, const std::string& src, const std::string& name);

        int GetUniformLocation(const std::string& name);
    };
}
