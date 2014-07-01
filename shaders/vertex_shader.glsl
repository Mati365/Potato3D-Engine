#version 400
layout(location = 0) in  vec4 Position;
layout(location = 1) in  vec3 Normal;
layout(location = 2) in  vec2 UV;
layout(location = 3) in  int	MTL_index;

out vec2		frag_uv;
out float	dist;
out float	mtl;

struct Matrix {
	mat4x4 	mvp;
};
uniform Matrix	matrix;

void main(void) {
	gl_Position = Position * matrix.mvp;
	dist = max(0, 1 - distance(gl_Position, vec4(0,0,0,1)) / 12);
	frag_uv = UV;
	mtl = MTL_index;
}