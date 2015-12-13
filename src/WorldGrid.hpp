#pragma once
#include <memory>
#include <vector>
#include "DrawableGrid.hpp"
#include "OpenGL/ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>

template<uint16_t Resolution>
class WorldTile {
public:
	const uint16_t resolution = Resolution;

	struct{uint16_t x,y;} index;

	glm::ivec2 getIndex() const {
		return {index.x, index.y};
	}
	static glm::ivec2 getScale() {
		return {Resolution, Resolution};
	}

	WorldTile<Resolution> &operator=(const WorldTile<Resolution> &rhs) {index = rhs.index; return *this;};

	WorldTile() = default;
	WorldTile(const WorldTile<Resolution> &rhs) {index = rhs.index;}
	WorldTile(uint16_t index_x, uint16_t index_y) : index{index_x, index_y} {}
};

template<uint16_t SizeX, uint16_t SizeY, uint16_t Resolution>
class WorldGrid {
public:
	const uint16_t resolution = Resolution;
	const struct{uint16_t x,y;} size {SizeX, SizeY};

	bool instance_data_changed = true;
	GLuint geometry_instance_vbo;

	std::shared_ptr<const DrawableGrid<>> geometry;
	
	WorldTile<Resolution> tiles[SizeX][SizeY];

	glm::vec2 camera_pos;
	glm::ivec2 offset;

	void setCameraPosition(const glm::vec2 &pos) {
		camera_pos = pos;
		offset = glm::ivec2(pos) / glm::ivec2{resolution, resolution};
		offset -= glm::ivec2{size.x/2, size.y/2};
	}

	void generateInstanceBuffer() {
		if (instance_data_changed) {
			struct{uint16_t data[3];} data[SizeX][SizeY];
			for (auto x = 0; x < SizeX; ++x) {
				for (auto y = 0; y < SizeY; ++y) {
					data[x][y] = {
						tiles[x][y].resolution,
						tiles[x][y].index.x, tiles[x][y].index.y
					};
				}
			}
			glBindBuffer(GL_ARRAY_BUFFER, geometry_instance_vbo);
			glBufferData(GL_ARRAY_BUFFER, SizeX * SizeY * sizeof(uint16_t)*3, data, GL_STATIC_DRAW);

			instance_data_changed = false;
		}
	}

	void draw(Graphics::OpenGL::ShaderProgram &program) {
		program.setUniformData(program.getUniformLocation("uOffset"), offset);
		this->generateInstanceBuffer();
		geometry->bind();
		geometry->drawInstanced(SizeX * SizeY);
	}

	WorldGrid(decltype(geometry) geometry) : geometry(geometry) {
		geometry->bind();
		glGenBuffers(1, &geometry_instance_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_instance_vbo);
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 3, GL_SHORT, GL_FALSE, nullptr);
		glVertexAttribDivisor(1, 1);

		for (auto x = 0; x < SizeX; ++x) {
			for (auto y = 0; y < SizeY; ++y) {
				tiles[x][y] = WorldTile<Resolution>(x,y);
			}
		}
	}
};
