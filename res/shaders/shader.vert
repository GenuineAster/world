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
out vec4 vPosition;
out vec3 vNormal;

// vec2 getScale(){return vec2(scale_index.xx);}
// vec2 getOffset(){return vec2(scale_index.yz + vec2(uOffset)) * getScale();}
float getHeight(in vec2 pos){return texture(uHeightmap, position).r;}
vec3 getPosition(in vec2 pos){return vec3(pos, getHeight(pos));}

// void fixDepth(inout vec4 P) {
// 	P.z = 2.0*log(P.w/uNear)/log(uFar/uNear) - 1; 
// 	P.z *= P.w;
// }

void main() {
	vec2 plane_pos = position;// * getScale() + getOffset();

	{
		vec2 offset = vec2(1.0/256.0, 1.0/256.0);// * getScale();
		vec4 positions[4] = vec4[4](
			uView * uModel * vec4(getPosition(vec2(plane_pos.x-offset.x, plane_pos.y-offset.y)), 1.0),
			uView * uModel * vec4(getPosition(vec2(plane_pos.x-offset.x, plane_pos.y+offset.y)), 1.0),
			uView * uModel * vec4(getPosition(vec2(plane_pos.x+offset.x, plane_pos.y-offset.y)), 1.0),
			uView * uModel * vec4(getPosition(vec2(plane_pos.x+offset.x, plane_pos.y+offset.y)), 1.0)
		);
		vNormal = normalize(cross(
			vec3(positions[3] - positions[0]),
			vec3(positions[2] - positions[1])
		));
		vNormal = vNormal;
		// vNormal = vec3(uView * uModel * vec4(vec3(0.0, 0.0, -1.0), 1.0));
	}

	gl_Position = uView * uModel * vec4(getPosition(plane_pos) , 1.0);
	vPosition = gl_Position;
	gl_Position = uProjection * gl_Position;
	// fixDepth(gl_Position);
}
