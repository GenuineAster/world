#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

template<typename Tindex=uint16_t, int index_type=GL_UNSIGNED_SHORT>
class DrawableGrid
{
private:
	GLuint vao, vbo, ebo;
	std::vector<glm::vec2> vertices;
	std::vector<Tindex> indices;

public:
	void bind() const {
		glBindVertexArray(vao);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}
	void draw() const {
		glDrawElements(GL_TRIANGLES, indices.size(), index_type, nullptr);
	}
	void drawInstanced(const uint32_t instances) const {
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), index_type, nullptr, instances);
	}
	DrawableGrid(int divs) {
		vertices.reserve((divs+1) * (divs+1));
		indices.reserve(divs * divs * 6);

		for (auto i=0; i <= divs; ++i) {
			for (auto j=0; j <= divs; ++j) {
				auto vertex = glm::vec2{i, j} / glm::vec2{divs, divs};
				// vertex -= glm::vec3{1.0, 1.0, 0.0};
				vertices.push_back(vertex);

				if (i < divs && j < divs) {
					indices.push_back((i+0) + (j+0) * (divs+1));
					indices.push_back((i+1) + (j+0) * (divs+1));
					indices.push_back((i+0) + (j+1) * (divs+1));

					indices.push_back((i+1) + (j+1) * (divs+1));
					indices.push_back((i+0) + (j+1) * (divs+1));
					indices.push_back((i+1) + (j+0) * (divs+1));
				}
			}
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Tindex) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	~DrawableGrid() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};
