#version 330 core

layout(location=0) in vec2 position;
// layout(location=1) in ivec3 scale_index;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
// uniform ivec2 uOffset;
uniform float uNear, uFar;
uniform sampler2D uHeightmap;


out float vHeight;
out vec2 vTexcoords;

// vec2 getScale(){return vec2(scale_index.xx);}
// vec2 getOffset(){return vec2(scale_index.yz + vec2(uOffset)) * getScale();}
float getHeight(in vec2 pos){return texture(uHeightmap, position).r;}
vec3 getPosition(in vec2 pos){return vec3(pos, getHeight(pos));}

void fixDepth(inout vec4 P) {
	P.z = 2.0*log(P.w/uNear)/log(uFar/uNear) - 1; 
	P.z *= P.w;
}

void main() {
	vec2 plane_pos = position;// * getScale() + getOffset();
	vTexcoords = position * vec2(64);

	gl_Position = uView * uModel * vec4(getPosition(plane_pos) , 1.0);
	gl_Position = uProjection * gl_Position;
	fixDepth(gl_Position);
}
