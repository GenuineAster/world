#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 vTexcoords[];
in float vHeight[];

out vec4 gPosition;
out vec2 gTexcoords;
out float gHeight;
out vec3 gNormal;

void main() {
	gNormal = normalize(cross(
		vec3(gl_in[1].gl_Position - gl_in[0].gl_Position),
		vec3(gl_in[2].gl_Position - gl_in[0].gl_Position)
	));

	gl_Position = gl_in[0].gl_Position;
	gPosition = gl_in[0].gl_Position;
	gTexcoords = vTexcoords[0];
	gHeight = vHeight[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	gPosition = gl_in[1].gl_Position;
	gTexcoords = vTexcoords[1];
	gHeight = vHeight[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	gPosition = gl_in[2].gl_Position;
	gTexcoords = vTexcoords[2];
	gHeight = vHeight[2];
	EmitVertex();

	EndPrimitive();
}
