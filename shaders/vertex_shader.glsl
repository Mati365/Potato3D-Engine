#version 400
layout(location = 0) in  vec4 	Position;
layout(location = 1) in  vec3 	Normal;
layout(location = 2) in  vec2 	UV;
layout(location = 3) in  int	MTL_index;

out 	vec2		frag_uv;
out		vec3		frag_normal;
out 	vec3		frag_pos;
out 	float		mtl;

struct Matrix {
	mat4	 	mvp;
	mat4		m;
	mat3		normal;
};
uniform Matrix	matrix;

void main(void) {
	gl_Position 	= 	Position * matrix.mvp;
	frag_normal		= 	Normal * matrix.normal;
	frag_pos		=	vec3(Position * matrix.m);

	frag_uv = UV;
	mtl = MTL_index;
}
