#version 330 core

in vec3 gNormal;
in vec2 gTexcoords;

uniform sampler2D uTex;

out vec4 fColor;

void main() {
	// Directional light
	vec3 to_surface = vec3(0.0, 0.0, -1.0);
	vec3 normal = normalize(gNormal);
	float brightness = dot(to_surface, normal);

	fColor = vec4(texture(uTex, gTexcoords).rgb * brightness, 1.0);
}
