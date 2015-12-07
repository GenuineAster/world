#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Graphics
{
	namespace OpenGL
	{
		class Texture;

		/*! \brief A wrapper around the OpenGL framebuffer API
		 */
		class Framebuffer
		{
		private:

			/*! \brief The internal OpenGL framebuffer object
			 */
			GLuint m_framebuffer;
		public:

			/*! \brief Creates the internal frambuffer object
			 */
			void create();

			/*! \brief Binds a 1D texture to an attachment on the framebuffer
			 *  \param[in] attachment the point to attach to
			 *  \param[in] tex the texture to attach
			 *  \param[in] level the LOD level to use
			 */
			void bindTexture1D(GLenum attachment, Texture &tex, GLint level);

			/*! \brief Binds a 2D texture to an attachment on the framebuffer
			 *  \param[in] attachment the point to attach to
			 *  \param[in] tex the texture to attach
			 *  \param[in] level the LOD level to use
			 */
			void bindTexture2D(GLenum attachment, Texture &tex, GLint level);

			/*! \brief Binds a 3D texture to an attachment on the framebuffer
			 *  \param[in] attachment the point to attach to
			 *  \param[in] tex the texture to attach
			 *  \param[in] level the LOD level to use
			 *  \param[in] layer the layer of the 3D texture to use
			 */
			void bindTexture3D(
				GLenum attachment,
				Texture &tex,
				GLint level,
				GLint layer
			);

			/*! \brief Binds fragment shader outputs to the framebuffer's bound color
			 *         buffers.
			 *  \param[in] buffers A vector of buffers to bind. Their position in the
			 *                     vector corresponds to the 2nd argument in
			 *                     [ShaderProgram::bindFragDataLocation].
			 *  \sa ShaderProgram::bindFragDataLocation
			 */
			void drawBuffers(std::vector<GLenum> buffers);

			/*! \brief Destroys the internal frambuffer object
			 */
			void destroy();

			/*! \brief Returns the internal framebuffer object.
			 *  \returns the internal framebuffer object
			 */
			operator GLuint();

			/*! \brief Returns whether the internal frambuffer object is valid or not.
			 *  \returns whether the internal frambuffer object is valid or not.
			 */
			explicit operator bool() const;

			/*! \brief Returns whether the internal frambuffer object is valid or not.
			 *  \returns whether the internal frambuffer object is valid or not.
			 */
			bool isValid() const;

			/*! \brief binds the given framebuffer (or none if nullptr)
			           to GL_FRAMEBUFFER
			    \param[in] framebuffer the framebuffer to bind
			 */
			static void bind(Framebuffer *framebuffer);

			/*! \brief binds the given framebuffer (or none if nullptr)
			           to the given bind point
			    \param[in] bind_point the bind point to bind to
			    \param[in] framebuffer the framebuffer to bind
			 */
			static void bind(GLenum bind_point, Framebuffer *framebuffer);

			/*! \brief Binds the framebuffer to the given bind point
			 *        (default is GL_FRAMEBUFFER)
			 * \param[in] bind_point the bind point to bind to
			 */
			void bind(GLenum bind_point = GL_FRAMEBUFFER);

			/*! \brief Constructs the framebuffer object.
			 */
			Framebuffer();

			/*! \brief Destroys the framebuffer object.
			 */
			~Framebuffer();

		};
	}
}
