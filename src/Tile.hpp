#pragma once
#include "DrawableGrid.hpp"
#include "OpenGL/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Tile
{
public:
	DrawableGrid<> *grid;
	Graphics::OpenGL::Texture heightmap;

	void loadFromFile(const std::string &filename) {
		int x,y,comp;
		uint8_t *data = stbi_load(filename.c_str(), &x, &y, &comp, 0);
		heightmap.create();
		heightmap.bind(GL_TEXTURE0, GL_TEXTURE_2D);
		heightmap.texImage2D(0, GL_R8, x, y, GL_RED, GL_UNSIGNED_BYTE, data);
	}

	void draw() {
		grid->draw();
	}

	Tile(DrawableGrid<> *grid) : grid(grid) {

	}
};

