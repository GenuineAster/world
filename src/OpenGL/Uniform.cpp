#include "OpenGL/Uniform.hpp"

namespace Graphics {
	namespace OpenGL {

		Uniform::operator std::string() const
		{
			return m_name;
		}

		Uniform::operator GLint() const
		{
			return m_uniform;
		}

		Uniform::operator bool() const
		{
			return this->isValid();
		}

		bool Uniform::isValid() const
		{
			return m_uniform != -1;
		}

		Uniform::Uniform(std::string name, GLint uniform) :
			m_name(name),
			m_uniform(uniform)
		{}
	}
}
