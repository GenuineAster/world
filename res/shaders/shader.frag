#version 330 core

in float vHeight;
in vec4 gPosition;
in vec3 gNormal;
in vec2 gTexcoords;

uniform mat4 uView;
uniform sampler2D uTex;

out vec4 fColor;

void main() {
	vec3 light_pos = vec3(0.5, 0.5, 3.0);
	vec3 fragment_pos = vec3(inverse(uView) * vec4(gPosition.xyz, 1.0));
	vec3 to_surface = normalize(light_pos - fragment_pos);
	vec3 normal = mat3(inverse(uView)) * gNormal;
	float brightness = 1.0 - ((dot(to_surface, normal) + 1.0) / 2.0);
	brightness = clamp(brightness, 0.0, 1.0);

	fColor = texture(uTex, gTexcoords);
	fColor.rgb *= brightness;
}
