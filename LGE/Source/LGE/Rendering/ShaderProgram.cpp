#include "ShaderProgram.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.inl>

namespace LGE
{
	ShaderProgram::ShaderProgram(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
		: m_Id(0)
	{
		m_Id = CreateShader(vertexShaderSrc, fragmentShaderSrc);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_Id);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_Id);
	}

	void ShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void ShaderProgram::SetUniform1i(const std::string& name, int v)
	{
		glUniform1i(GetUniformLocation(name), v);
	}

	void ShaderProgram::SetUniform1f(const std::string& name, float v)
	{
		glUniform1f(GetUniformLocation(name), v);
	}

	void ShaderProgram::SetUniform2f(const std::string& name, float v0, float v1)
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}

	void ShaderProgram::SetUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}

	void ShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void ShaderProgram::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	unsigned int ShaderProgram::CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	unsigned int ShaderProgram::CompileShader(unsigned int type, const std::string& src)
	{
		unsigned int id = glCreateShader(type);
		const char* cSrc = src.c_str();
		glShaderSource(id, 1, &cSrc, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile "
				<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< " shader" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	int ShaderProgram::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) == m_UniformLocationCache.end())
		{
			m_UniformLocationCache[name] = glGetUniformLocation(m_Id, name.c_str());
		}

		return m_UniformLocationCache.at(name);
	}
}
