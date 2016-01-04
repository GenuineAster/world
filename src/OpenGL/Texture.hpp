#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>


namespace Graphics
{
	namespace OpenGL
	{
		/*! \brief A wrapper around OpenGL's texture API
		 */

		class Texture
		{
		private:
			/*! \brief The internal OpenGL texture object
			 */
			GLuint m_texture;

			/*! \brief The internal OpenGL texture binding
			 */
			GLenum m_tex_num;

			/*! \brief The internal OpenGL texture target (type)
			 */
			GLenum m_target;

			/*! \brief The internal construction flag
			 */
			bool m_valid;

		public:
			/*! \brief Binds the texture object to the given texture unit and target
			 *  \param[in] tex_num the texture number to activate
			 *  \param[in] target the OpenGL target to bind to
			 */
			void bind(GLenum tex_num, GLenum target);

			/*! \brief Creates the texture
			 */
			void create();

			/*! \brief Uploads 1D data to the texture.
			 *  \param[in] level the LOD number, default is 0
			 *  \param[in] internal_format the texture's internal format
			 *  \param[in] width the texture's width
			 *  \param[in] height the texture's height
			 *  \param[in] format the pixel data format
			 *  \param[in] type the way the pixel data format is stored
			 *  \param[in] data the pixel data to upload
			 */
			void texImage1D(
				GLint level,
				GLint internal_format,
				GLsizei width,
				GLenum format,
				GLenum type,
				const GLvoid *data
			);

			/*! \brief Uploads 2D data to the texture.
			 *  \param[in] level the LOD number, default is 0
			 *  \param[in] internal_format the texture's internal format
			 *  \param[in] width the texture's width
			 *  \param[in] height the texture's height
			 *  \param[in] format the pixel data format
			 *  \param[in] type the way the pixel data format is stored
			 *  \param[in] data the pixel data to upload
			 */
			void texImage2D(
				GLint level,
				GLint internal_format,
				GLsizei width,
				GLsizei height,
				GLenum format,
				GLenum type,
				const GLvoid *data
			);

			/*! \brief Uploads 3D data to the texture.
			 *  \param[in] level the LOD number, default is 0
			 *  \param[in] internal_format the texture's internal format
			 *  \param[in] width the texture's width
			 *  \param[in] height the texture's height
			 *  \param[in] depth the texture's depth
			 *  \param[in] format the pixel data format
			 *  \param[in] type the way the pixel data format is stored
			 *  \param[in] data the pixel data to upload
			 */
			void texImage3D(
				GLint level,
				GLint internal_format,
				GLsizei width,
				GLsizei height,
				GLsizei depth,
				GLenum format,
				GLenum type,
				const GLvoid *data
			);


			/*! \brief Gets the texture's bound unit
			 * \returns the OpenGL texture bind point tex unit
			 */
			GLenum getTextureNum() const;

			/*! \brief Gets the texture target (type)
			 * \returns the OpenGL texture bind point target
			 */
			GLenum getTarget() const;

			/*! \brief Gets the texture name
			 * \returns the OpenGL texture name
			 */
			GLuint getTextureName() const;

			/*! \brief Destroys the texture
			 */
			void destroy();

			/*! \brief Returns the internal OpenGL texture object
			 *  \returns the internal OpenGL texture object
			 */
			explicit operator GLuint();

			/*! \brief Returns whether the texture is correctly formed or not
			 *  \returns whether the texture is correctly formed or not
			 */
			explicit operator bool() const;

			/*! \brief Returns whether the texture is correctly formed or not
			 *  \returns whether the texture is correctly formed or not
			 */
			bool isValid() const;

			/*! \brief Move assignment
			 */
			Texture &operator=(Texture &&other);

			/*! \brief Constructs the texture
			 */
			Texture();

			/*! \note Move constructor.
			 *  \param[in] other the Texture to move from
			 */
			Texture(Texture &&other);

			/*! \brief Destroys the texture object
			 */
			~Texture();
		};
	}
}
