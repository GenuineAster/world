#include "OpenGL/Texture.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		void Texture::bind(GLuint tex_num, GLenum target)
		{
			m_tex_num = tex_num;
			m_target = target;
			glActiveTexture(m_tex_num);
			glBindTexture(m_target, m_texture);
		}

		void Texture::create()
		{
			if ( ! this->isValid()) {
				glGenTextures(1, &m_texture);
				m_valid = true;
			}
		}

		void Texture::texImage1D(
			GLint level,
			GLint internal_format,
			GLsizei width,
			GLenum format,
			GLenum type,
			const GLvoid *data
		) {
			if ( ! this->isValid())
				return;

			glActiveTexture(m_tex_num);
			// Upload texture data to the texture
			glTexImage1D(m_target,
				level, internal_format,
				width, 0,
				format, type, data
			);
			// Set basic "default" texture params
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		void Texture::texImage2D(
			GLint level,
			GLint internal_format,
			GLsizei width,
			GLsizei height,
			GLenum format,
			GLenum type,
			const GLvoid *data
		) {
			if ( ! this->isValid())
				return;

			glActiveTexture(m_tex_num);
			// Upload texture data to the texture
			glTexImage2D(m_target,
				level, internal_format,
				width, height, 0,
				format, type, data
			);
			// Set basic "default" texture params
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		void Texture::texImage3D(
			GLint level,
			GLint internal_format,
			GLsizei width,
			GLsizei height,
			GLsizei depth,
			GLenum format,
			GLenum type,
			const GLvoid *data
		) {
			if ( ! this->isValid())
				return;

			glActiveTexture(m_tex_num);
			// Upload texture data to the texture
			glTexImage3D(m_target,
				level, internal_format,
				width, height, depth, 0,
				format, type, data
			);
		}

		GLenum Texture::getTarget() const
		{
			return m_target;
		}

		GLenum Texture::getTextureNum() const
		{
			return m_tex_num;
		}

		void Texture::destroy()
		{
			if (this->isValid()) {
				glDeleteTextures(1, &m_texture);
				m_valid = false;
			}
			m_texture = -1;
		}

		Texture::operator GLuint()
		{
			return m_texture;
		}

		Texture::operator bool() const
		{
			return this->isValid();
		}

		bool Texture::isValid() const
		{
			return glIsTexture(m_texture) || m_valid;
		}

		Texture::Texture() : m_texture(-1), m_tex_num(GL_TEXTURE0)
		{
			this->create();
		}

		Texture::~Texture()
		{
			this->destroy();
		}
	}
}
