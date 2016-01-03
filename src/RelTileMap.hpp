#pragma once
#include "Tile.hpp"
#include <glm/glm.hpp>

class RelTileMap
{
private:
	int16_t m_radius;
	std::vector<glm::tvec2<int16_t>> m_indices;
public:
	void create(const int16_t radius) {
		m_radius = radius;

		m_indices.clear();
		m_indices.reserve(4 * m_radius * m_radius);
		for (auto i = -m_radius; i < m_radius; ++i) {
			for (auto j = -m_radius; j < m_radius; ++j) {
				m_indices.emplace_back(i, j);
			}
		}
		std::sort(m_indices.begin(), m_indices.end(), [](const auto &a, const auto &b){
			return (a.x * a.x) + (a.y * a.y) < (b.x * b.x) + (b.y * b.y);
		});
	}

	const auto &getIndices() const {
		return m_indices;
	}

	RelTileMap(const int16_t radius=0) {this->create(radius);}
};

