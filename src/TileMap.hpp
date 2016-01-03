#pragma once
#include <regex>
#include <fstream>
#include <exception>
#include <unordered_map>

#include <json/json.h>

#include "Tile.hpp"

Tile createTileFromJson(const Json::Value &data) {
	Tile tile;
	int16_t index_x = data["x"].asInt();
	int16_t index_y = data["y"].asInt();

	tile.setIndex(index_x, index_y);

	if (!data["default"].isNull()) {
		tile.setDefault(data["default"].asBool());
	}

	if (!data["heightmap"].isNull()) {
		tile.setHeightmap(data["heightmap"].asString());
	}

	return tile;
}

class TileMap
{
private:
	static constexpr uint32_t getKey(const int16_t x, const int16_t y) { return (uint32_t(x)<<16) | uint32_t(y); }
	
	Tile m_default_tile;
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

	Tile &getTileOrDefault(const int16_t x, const int16_t y) {
		auto tile = getTile(x, y);
		return tile ? *tile : m_default_tile;
	}

	auto &getTiles() {
		return m_tiles;
	}

	const auto &getTiles() const {
		return m_tiles;
	}

	Tile &getDefaultTile() {
		return m_default_tile;
	}

	const Tile &getDefaultTile() const {
		return m_default_tile;
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

		m_default_tile = createTileFromJson(root["default_tile"]);

		auto tiles = root["tiles"];

		for (auto &tile_data : tiles) {
			int16_t index_x = tile_data["x"].asInt();
			int16_t index_y = tile_data["y"].asInt();

			this->createTile(index_x, index_y) = createTileFromJson(tile_data);
		}
	}
};
