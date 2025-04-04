#include "ShaderProgram.h"

#include <iostream>

namespace Renderer {
		
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "VERTEX SHADER compile-time error" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr  << "FRAGMENT SHADER compile-time error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infolog[1024];
			glGetProgramInfoLog(m_ID, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: link-time error:\n" << infolog << std::endl;
		}
		else
		{
			m_isCompiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infolog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Compile-time error:\n" << infolog << std::endl;
			return false;
		}
		return true;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& ShaderProgram) noexcept
	{
		glDeleteShader(m_ID);
		m_ID = ShaderProgram.m_ID;
		m_isCompiled = ShaderProgram.m_isCompiled;

		ShaderProgram.m_ID = 0;
		ShaderProgram.m_ID = false;
		return *this;
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& ShaderProgram) noexcept
	{
		m_ID = ShaderProgram.m_ID;
		m_isCompiled = ShaderProgram.m_isCompiled;

		ShaderProgram.m_ID = 0;
		ShaderProgram.m_ID = false;
	}
}