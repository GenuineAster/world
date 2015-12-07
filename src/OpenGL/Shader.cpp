#include "OpenGL/Shader.hpp"

#include <memory>
#include <fstream>
#include <stdexcept>

namespace Graphics
{
	namespace OpenGL
	{
		void Shader::create(GLenum type)
		{
			// Only create the shader if the current one is destroyed
			if ( ! this->isValid()) {
				m_shader = glCreateShader(type);
			}
		}

		void Shader::setSource(std::string src)
		{
			auto c_src = src.c_str();
			glShaderSource(m_shader, 1, &c_src, NULL);
		}

		void Shader::setSourceFromFile(std::string filename)
		{
			std::ifstream file(filename);
			if( ! file) {
				return;
			}

			std::string src;
			for(std::string line; std::getline(file, line); ) {
				src += line + '\n';
			}
			auto c_src = src.c_str();
			glShaderSource(m_shader, 1, &c_src, NULL);
		}

		std::string Shader::getSource() const
		{
			GLint source_length;
			glGetShaderiv(m_shader, GL_SHADER_SOURCE_LENGTH, &source_length);

			auto buff = std::make_unique<char[]>(source_length);

			glGetShaderSource(m_shader, source_length, NULL, buff.get());

			return buff.get();
		}

		void Shader::compile()
		{
			glCompileShader(m_shader);

			GLint status;
			glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);

			if (GL_FALSE == status) {
				GLint error_length;
				glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &error_length);

				auto error_buf = std::make_unique<char[]>(error_length);
				glGetShaderInfoLog(m_shader, error_length, NULL, error_buf.get());

				throw std::runtime_error(std::string{"Error compiling shader:\n"} + error_buf.get());
			}
		}

		void Shader::destroy()
		{
			// Only destroy the shader if the current one is actually a shader
			if (this->isValid()) {
				glDeleteShader(m_shader);
			}
			m_shader = -1;
		}

		Shader::operator GLuint()
		{
			return m_shader;
		}

		Shader::operator bool() const
		{
			return this->isValid();
		}

		bool Shader::isValid() const
		{
			return glIsShader(m_shader);
		}

		Shader::Shader()
		{
			m_shader = -1;
		}

		Shader::Shader(Shader &&other)
		{
			this->m_shader = other.m_shader;
			other.m_shader = -1;
		}

		Shader::Shader(GLenum type) : Shader()
		{
			this->create(type);
		}

		Shader::~Shader()
		{
			this->destroy();
		}
	}
}
