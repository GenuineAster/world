#pragma once
#include "DrawableGrid.hpp"
#include "OpenGL/Texture.hpp"

#include "stb_image.h"

class Tile
{
private:
	bool m_default = true;
	glm::tvec2<int16_t> m_index;

	std::string m_heightmap_path;
	Graphics::OpenGL::Texture m_heightmap;
public:
	Tile &setIndex(const int16_t x, const int16_t y) {
		m_index = {x, y};
		return *this;
	}

	glm::tvec2<int16_t> getIndex() const {
		return m_index;
	}

	Tile &setDefault(const bool def) {
		m_default = def;
		return *this;
	}

	auto getDefault() const {
		return m_default;
	}

	Tile &setHeightmap(const std::string &path) {
		m_default = false;

		m_heightmap.create();
		
		int x,y,comp;
		uint8_t *stbi_data = stbi_load(path.c_str(), &x, &y, &comp, 1);

		if (stbi_data) {
			m_heightmap_path = path;
			m_heightmap.bind(GL_TEXTURE1, GL_TEXTURE_2D);
			m_heightmap.texImage2D(0, GL_R8, x, y, GL_RED, GL_UNSIGNED_BYTE, stbi_data);
			stbi_image_free(stbi_data);
		}	

		return *this;
	}

	auto &getHeightmap() {
		return m_heightmap;
	}

	const auto &getHeightmap() const {
		return m_heightmap;
	}

	Tile() = default;

	Tile(Tile &&other) {
		this->m_default = other.m_default;
		this->m_index = other.m_index;

		this->m_heightmap_path = other.m_heightmap_path;
		this->m_heightmap = std::move(other.m_heightmap);
	}
};

