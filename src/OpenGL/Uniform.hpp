#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Graphics
{
	namespace OpenGL
	{
		/*! \brief A wrapper around the OpenGL uniform API
		 */
		class Uniform
		{
		private:
			friend class ShaderProgram;

			/*! \brief The name of the uniform
			 */
			std::string m_name;

			/*! \brief the internal OpenGL uniform object
			 */
			GLint m_uniform;

		public:

			/*! \brief Gets the uniform's name
			 *  \returns the uniform's name
			 */
			operator std::string() const;

			/*! \brief Gets the internal OpenGL uniform object
			 *  \returns the internal OpenGL uniform object
			 */
			operator GLint() const;

			/*! \brief Returns whether the uniform was correctly formed or not
			 *  \returns whether the uniform was correctly formed or not
			 */
			explicit operator bool() const;

			/*! \brief Returns whether the uniform was correctly formed or not
			 *  \returns whether the uniform was correctly formed or not
			 */
			bool isValid() const;

			/*! \brief Constructs the uniform
			 *  \param[in] name the uniform's name
			 *  \param[in] uniform the uniform's OpenGL representation
			 */
			Uniform(std::string name="", GLint uniform=-1);
		};
	}
}
