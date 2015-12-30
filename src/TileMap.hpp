#pragma once
#include <regex>
#include <fstream>
#include <exception>
#include <unordered_map>

#include <json/json.h>

#include "Tile.hpp"

class TileMap
{
private:
	static constexpr uint32_t getKey(const int16_t x, const int16_t y) { return (uint32_t(x)<<16) | uint32_t(y); }
	
	std::vector<Tile> m_tiles;
	std::unordered_map<uint32_t, std::size_t> m_map;

	glm::vec2 m_tile_size;

public:
	const auto getTileSize() const noexcept {
		return m_tile_size;
	}
	
	Tile *getTile(const int16_t x, const int16_t y) {
		uint32_t key = getKey(x, y);
		auto tile = m_map.find(key);
		if (tile !=  m_map.end()) {
			return &m_tiles[tile->second];
		}
		return nullptr;
	}

	auto &getTiles() {
		return m_tiles;
	}

	const auto &getTiles() const {
		return m_tiles;
	}

	Tile &createTile(const int16_t x, const int16_t y) {
		if (auto tile = getTile(x, y)) {
			return *tile;
		} else {
			m_tiles.emplace_back();
			m_map[getKey(x, y)] = m_tiles.size() - 1;
			return m_tiles.back();
		}
	}

	void loadFromFile(const char* path) {
		std::ifstream file(path);
		if (!file) {
			throw std::runtime_error(std::string{"TileMap::loadFromFile(\""} + path + "\"): could not open file.");
		}

		Json::Value root;
		Json::Reader{}.parse(file, root);

		file.close();

		if (root.isNull()) {
			throw std::runtime_error(std::string{"TileMap::loadFromFile(\""} + path + "\"): null JSON data.");
		}

		m_tile_size = {
			root["tile_size"][0].asFloat(),
			root["tile_size"][1].asFloat()
		};

		auto tiles = root["tiles"];

		for (auto &tile_data : tiles) {
			int16_t index_x = tile_data["x"].asInt();
			int16_t index_y = tile_data["y"].asInt();

			auto &tile = this->createTile(index_x, index_y);
			tile.setIndex(index_x, index_y);

			if (!tile_data["default"].isNull()) {
				tile.setDefault(tile_data["default"].asBool());
			}

			if (!tile_data["heightmap"].isNull()) {
				tile.setHeightmap(tile_data["heightmap"].asString());
			}

		}
	}
};
