#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 vTexcoords[];

uniform mat4 uView;
uniform mat4 uProjection;
uniform float uNear, uFar;

out vec2 gTexcoords;
out vec3 gNormal;

void fixDepth(inout vec4 P) {
	P.z = 2.0*log(P.w/uNear)/log(uFar/uNear) - 1; 
	P.z *= P.w;
}

void main() {
	gNormal = normalize(cross(
		vec3(gl_in[1].gl_Position - gl_in[0].gl_Position),
		vec3(gl_in[2].gl_Position - gl_in[0].gl_Position)
	));

	mat4 transform = uProjection * uView;

	gl_Position = transform * gl_in[0].gl_Position;
	gTexcoords = vTexcoords[0];
	EmitVertex();

	gl_Position = transform * gl_in[1].gl_Position;
	gTexcoords = vTexcoords[1];
	EmitVertex();

	gl_Position = transform * gl_in[2].gl_Position;
	gTexcoords = vTexcoords[2];
	EmitVertex();

	EndPrimitive();
}
