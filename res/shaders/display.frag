#version 330 core

in vec2 vPosition;
in vec2 vTexcoords;

uniform sampler2D uColor;

out vec4 fColor;

void main() {
	fColor = texture(uColor, vTexcoords);
}
