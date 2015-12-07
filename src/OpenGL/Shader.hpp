#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Graphics
{
	namespace OpenGL
	{
		/*! \brief A wrapper around OpenGL's shader class
		 */

		class Shader
		{
		private:
			/*! \brief The internal OpenGL shader object
			 */
			GLuint m_shader;

		public:
			/*! \brief Creates the shader object with the given type
			 *  \param[in] type the type of shader to create
			 */
			void create(GLenum type);

			/*! \brief Sets the shader's source
			 *  \param[in] src the string containing the shader's source
			 */
			void setSource(std::string src);

			/*! \brief Reads the shader's source from a file
			 *  \param[in] filename the path to the shader's source file
			 */
			void setSourceFromFile(std::string filename);

			/*! \brief Gets the shader's source
			 *  \returns the shader's source
			 */
			std::string getSource() const;

			/*! \brief Compiles the shader
			 */
			void compile();

			/*! \brief Destroys the shader
			 */
			void destroy();

			/*! \brief Returns the internal OpenGL shader object
			 *  \returns the internal OpenGL shader object
			 */
			operator GLuint();

			/*! \brief Returns whether the shader is correctly formed or not
			 *  \returns whether the shader is correctly formed or not
			 */
			explicit operator bool() const;

			/*! \brief Returns whether the shader is correctly formed or not
			 *  \returns whether the shader is correctly formed or not
			 */
			bool isValid() const;

			/*! \note This constructor does not create the shader object, you must do
			 *        this manually.
			 */
			Shader();

			/*! \note Move constructor. Does not delete the shader moved from.
			 *  \param[in] other the Shader to move from
			 */
			Shader(Shader &&other);

			/*! \brief Constructs the OpenGL shader with a given type.
			 *  \param[in] type the type to construct the shader with
			 */
			Shader(GLenum type);

			/*! \brief Destroys the shader object
			 */
			~Shader();
		};
	}
}
