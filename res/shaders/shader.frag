#version 330 core

in float vHeight;
in vec4 vPosition;
in vec3 vNormal;

uniform mat4 uView;

out vec4 fColor;

void main() {
	vec3 light_pos = vec3(0.0, 3.0, 0.0);
	vec3 fragment_pos = vec3(inverse(uView) * vPosition);
	vec3 to_surface = normalize(light_pos - fragment_pos);
	vec3 normal = mat3(inverse(uView)) * vNormal;
	float brightness = dot(to_surface, normal);
	brightness = clamp(brightness, 0.0, 1.0);

	fColor = vec4(1.0, 1.0, 1.0, 1.0);
	fColor.rgb *= brightness;
}
