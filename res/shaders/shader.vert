#version 330 core

layout(location=0) in vec2 position;

uniform mat4 uModel;
uniform sampler2D uHeightmap;

out vec2 vTexcoords;

float getHeight(in vec2 pos){return texture(uHeightmap, position).r;}
vec4 getPosition(in vec2 pos){return vec4(pos, getHeight(pos), 1.0);}

void main() {
	vTexcoords = position * vec2(64);
	gl_Position = uModel * getPosition(position);
}
