// #include "DrawableGrid.hpp"

// void DrawableGrid::bind() const {
// 	glBindVertexArray(vao);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// }

// void DrawableGrid::draw() const {
// 	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
// }

// void DrawableGrid::drawInstanced(const uint32_t instances) const {
// 	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, instances);
// }

// DrawableGrid::DrawableGrid(int divs) {
// 	vertices.reserve((divs+1) * (divs+1));
// 	indices.reserve(divs * divs * 6);

// 	for (auto i=0; i <= divs; ++i) {
// 		for (auto j=0; j <= divs; ++j) {
// 			auto vertex = glm::vec2{i, j} / glm::vec2{divs, divs};
// 			// vertex -= glm::vec3{1.0, 1.0, 0.0};
// 			vertices.push_back(vertex);

// 			if (i < divs && j < divs) {
// 				indices.push_back((i+0) + (j+0) * (divs+1));
// 				indices.push_back((i+1) + (j+0) * (divs+1));
// 				indices.push_back((i+0) + (j+1) * (divs+1));

// 				indices.push_back((i+1) + (j+1) * (divs+1));
// 				indices.push_back((i+1) + (j+0) * (divs+1));
// 				indices.push_back((i+0) + (j+1) * (divs+1));
// 			}
// 		}
// 	}

// 	glGenVertexArrays(1, &vao);
// 	glBindVertexArray(vao);

// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(vertices)::value_type) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

// 	glGenBuffers(1, &ebo);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(), indices.data(), GL_STATIC_DRAW);

// 	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
// }

// DrawableGrid::~DrawableGrid() {
// 	glDeleteVertexArrays(1, &vao);
// 	glDeleteBuffers(1, &vbo);
// 	glDeleteBuffers(1, &ebo);
// }
