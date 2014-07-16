#version 400
layout(location = 0) in  vec4 	Position;
layout(location = 1) in  vec3 	Normal;
layout(location = 2) in  vec2 	UV;
layout(location = 3) in  int	MTL_index;

out FragInfo {
	vec2		uv;
	vec3		normal;
	vec3		pos;
	vec3		cam;
	float		mtl;
} frag;

struct Matrix {
	mat4	 	mvp;
	mat4		m;
	mat3		normal;
	vec4		cam; // w model matrix
};
uniform Matrix	matrix;

void main(void) {
	gl_Position = 	Position * matrix.mvp;
	
	frag.normal	= 	Normal * matrix.normal;
	frag.pos	=	vec3(Position * matrix.m);
	frag.uv 	= 	UV;
	frag.mtl 	= 	MTL_index;
	frag.cam	=	matrix.cam.xyz;
}
