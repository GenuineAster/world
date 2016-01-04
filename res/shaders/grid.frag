#version 330 core

in vec2 vPosition;
in vec2 vTexcoords;

uniform sampler2D uColor;
uniform sampler2D uDepth;
uniform mat4 uView;
uniform mat4 uProj;

out vec4 fColor;

vec4 depth_to_world(vec2 screenspace, float depth) {
	vec4 position;
	position.xy = screenspace;
	position.z  = depth*2.0-1.0;
	position.w  = 1.0;
	position = inverse(uProj) * position;
	position /= position.w;
	return position;
}

void main() {
	float depth = texture(uDepth, vTexcoords).r;
	vec4 world_pos = inverse(uView) * depth_to_world(vTexcoords * 2.0 - 1.0, depth);
	

	vec2 border = fract(world_pos.xy / 1.0);
	border = max(border, vec2(1.0)-border);
	float bmul = step(0.997, max(border.x, border.y)) * step(depth, 0.999999);
	fColor = texture(uColor, vTexcoords) + vec4(1.0) * bmul;
}
