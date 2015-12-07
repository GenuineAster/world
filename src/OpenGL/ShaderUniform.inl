#pragma once

#include <glm/glm.hpp>

#include "OpenGL/Texture.hpp"
#include "OpenGL/ShaderProgram.hpp"
#include "OpenGL/Uniform.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		/*! \brief Uploads a 2D vector of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::vec2> (
			const Uniform &uniform,
			const glm::vec2 &data
		);

		/*! \brief Uploads a 2D vector of int to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::ivec2> (
			const Uniform &uniform,
			const glm::ivec2 &data
		);

		/*! \brief Uploads a 3D vector of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::vec3> (
			const Uniform &uniform,
			const glm::vec3 &data
		);

		/*! \brief Uploads a 3D vector of int to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::ivec3> (
			const Uniform &uniform,
			const glm::ivec3 &data
		);

		/*! \brief Uploads a 3D 3x3 matrix of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::mat3> (
			const Uniform &uniform,
			const glm::mat3 &data
		);

		/*! \brief Uploads a 3D 4x4 matrix of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::mat4> (
			const Uniform &uniform,
			const glm::mat4 &data
		);

		/*! \brief Uploads a texture sampler number to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<Texture> (
			const Uniform &uniform,
			const Texture &data
		);

		/*! \brief Uploads an integer to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLint> (
			const Uniform &uniform,
			const GLint &data
		);

		/*! \brief Uploads an unsigned integer to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLuint> (
			const Uniform &uniform,
			const GLuint &data
		);

		/*! \brief Uploads a float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLfloat> (
			const Uniform &uniform,
			const GLfloat &data
		);

		/*! \brief Uploads a double to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLdouble> (
			const Uniform &uniform,
			const GLdouble &data
		);
	}
}
