#define SHADER_PROGRAM_IMPL
#include "OpenGL/Shader.hpp"
#include "OpenGL/Uniform.hpp"
#include "OpenGL/ShaderProgram.hpp"

namespace Graphics
{
	namespace OpenGL
	{

		void ShaderProgram::create()
		{
			// Only create the shader program if the current one is invalid.
			if ( ! this->isValid()) {
				m_shader_program = glCreateProgram();
			}
		}

		void ShaderProgram::attach(Shader &shader)
		{
			glAttachShader(m_shader_program, shader);
		}

		void ShaderProgram::link()
		{
			glLinkProgram(m_shader_program);
		}

		void ShaderProgram::use()
		{
			glUseProgram(m_shader_program);
		}

		void ShaderProgram::destroy()
		{
			// Only destroy the shader program if the current one is valid
			if (this->isValid()) {
				glDeleteProgram(m_shader_program);
			}
			m_shader_program = -1;
		}

		void ShaderProgram::bindFragDataLocation(
			std::string name,
			GLuint color_number
		) {
			glBindFragDataLocation(m_shader_program, color_number, name.c_str());
		}

		Uniform ShaderProgram::getUniformLocation(std::string name)
		{
			return {name, glGetUniformLocation(m_shader_program, name.c_str())};
		}

		ShaderProgram::operator GLuint()
		{
			return m_shader_program;
		}

		ShaderProgram::operator bool() const
		{
			return this->isValid();
		}

		bool ShaderProgram::isValid() const
		{
			return glIsProgram(m_shader_program);
		}

		ShaderProgram::ShaderProgram()
		{
			m_shader_program = -1;
		}

		ShaderProgram::ShaderProgram(ShaderProgram &&other)
		{
			this->m_shader_program = other.m_shader_program;
			other.m_shader_program = -1;
		}

		ShaderProgram::~ShaderProgram()
		{
			this->destroy();
		}
	}
}
