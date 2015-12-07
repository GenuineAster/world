#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

namespace Graphics
{
	namespace OpenGL
	{
		class Shader;
		class Uniform;

		/*! \brief A wrapper around OpenGL's shader class
		 */

		class ShaderProgram
		{
		private:
			/*! \brief The internal OpenGL shader object
			 */
			GLuint m_shader_program;

		public:
			/*! \brief Creates the shader program object
			 */
			void create();

			/*! \brief Attaches the given shader to the program
			 *  \param[in] shader the shader to attach
			 */
			void attach(Shader &shader);

			/*! \brief Links the shader program
			 */
			void link();

			/*! \brief Uses/binds the shader program
			 */
			void use();

			/*! \brief Destroys the shader program
			 */
			void destroy();

			/*! \brief Binds a fragment shader output to a framebuffer color number
			 *  \param[in] name The name of the fragment shader output
			 *  \param[in] color_number The number of the color buffer binding to bind
			 *                          the output to.
			 */
			void bindFragDataLocation(std::string name, GLuint color_number);

			/*! \brief Gets the location of a uniform
			 *  \param[in] name the name of the uniform
			 *  \returns a [Uniform] object with the name and location of the uniform
			 */
			Uniform getUniformLocation(std::string name);

			/*! \brief Sets the uniform to the given data
			 *  \param[in] uniform the uniform to upload data to
			 *  \param[in] data the data to upload to the uniform
			 *  \tparam T the type of data to upload
			 */
			template<typename T>
			void setUniformData(const Uniform &uniform, const T &data);

			/*! \brief Returns the internal OpenGL shader program object
			 *  \returns the internal OpenGL shader program object
			 */
			operator GLuint();

			/*! \brief Returns whether the shader program is correctly formed or not
			 *  \returns whether the shader program is correctly formed or not
			 */
			explicit operator bool() const;

			/*! \brief Returns whether the shader program is correctly formed or not
			 *  \returns whether the shader program is correctly formed or not
			 */
			bool isValid() const;

			/*! \note This constructor does not construct the OpenGL shader program. You
			 *        must explicitly call ShaderProgram::create()
			 */
			ShaderProgram();

			/*! \note Move constructor. Does not delete the shader program moved from.
			 *  \param[in] other the program to move from
			 */
			ShaderProgram(ShaderProgram &&other);

			/*! \brief Destroys the shader object
			 */
			~ShaderProgram();
		};

	}
}
#ifndef SHADER_PROGRAM_IMPL
	#include "OpenGL/ShaderUniform.inl"
#endif
