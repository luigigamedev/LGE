#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace LGE
{
    // A ShaderProgram is composed of a vertex and a fragment shader
    class ShaderProgram
    {
    private:
        unsigned int m_Id; // Shader Program id
        std::unordered_map<std::string, int> m_UniformLocationCache;
    public:
        ShaderProgram(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
        ~ShaderProgram();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform1i(const std::string& name, int v);
        void SetUniform1f(const std::string& name, float v);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
    private:
        unsigned int CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
        unsigned int CompileShader(unsigned int type, const std::string& src);

        int GetUniformLocation(const std::string& name);
    };
}
