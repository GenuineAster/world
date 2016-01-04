#include "OpenGL/Framebuffer.hpp"
#include "OpenGL/Texture.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		void Framebuffer::create()
		{
			if ( ! this->isValid()) {
				glGenFramebuffers(1, &m_framebuffer);
			}
		}

		void Framebuffer::bindTexture1D(
			GLenum attachment,
			Texture &tex,
			GLint level
		) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glFramebufferTexture1D(
				GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.getTextureName(), level
			);
		}

		void Framebuffer::bindTexture2D(
			GLenum attachment,
			Texture &tex,
			GLint level
		) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.getTextureName(), level
			);
		}

		void Framebuffer::bindTexture3D(
			GLenum attachment,
			Texture &tex,
			GLint level,
			GLint layer
		) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glFramebufferTexture3D(
				GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.getTextureName(), level, layer
			);
		}

		void Framebuffer::drawBuffers(std::vector<GLenum> buffers)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
			glDrawBuffers(buffers.size(), buffers.data());
		}

		void Framebuffer::destroy()
		{
			if (this->isValid()) {
				glDeleteFramebuffers(1, &m_framebuffer);
			}
			m_framebuffer = -1;
		}

		Framebuffer::operator GLuint()
		{
			return m_framebuffer;
		}

		Framebuffer::operator bool() const {
			return this->isValid();
		}

		bool Framebuffer::isValid() const {
			return glIsFramebuffer(m_framebuffer);
		}

		void Framebuffer::bind(Framebuffer *framebuffer)
		{
			bind(GL_FRAMEBUFFER, framebuffer);
		}

		void Framebuffer::bind(GLenum bind_point, Framebuffer *framebuffer)
		{
			if (framebuffer) {
				glBindFramebuffer(bind_point, *framebuffer);
			}
			else {
				glBindFramebuffer(bind_point, 0);
			}
		}

		void Framebuffer::bind(GLenum bind_point)
		{
			Framebuffer::bind(bind_point, this);
		}

		Framebuffer::Framebuffer()
		{
			this->create();
		}

		Framebuffer::~Framebuffer()
		{
			this->destroy();
		}
	}
}
