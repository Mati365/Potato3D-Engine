#version 400
layout(location = 0) in  vec4 	Position;
layout(location = 1) in  vec3 	Normal;
layout(location = 2) in  vec2 	UV;
layout(location = 3) in  int	MTL_index;
layout(location = 4) in	 vec4	Tangent;

out FragInfo {
	vec2		uv;
	float		mtl;
	mat3		surface2view;
	vec3		pos; // w model view
	mat4		v;
} frag;

struct Matrix {
	mat4	 	mvp;
	mat4		m;
	mat4		v;
	mat3		normal;
	vec4		cam; // w model matrix
};
uniform Matrix	matrix;

void main(void) {
	frag.uv 	= 	UV;
	frag.mtl 	= 	MTL_index;
	frag.pos	=	(Position * matrix.m * matrix.v).xyz;
	frag.v		=	matrix.v;
	
	frag.surface2view[0] = normalize(matrix.normal * Tangent.xyz);
    frag.surface2view[2] = normalize(matrix.normal * Normal);
    frag.surface2view[1] = normalize(cross(frag.surface2view[2], frag.surface2view[0]));
    
    gl_Position = 	Position * matrix.mvp;
}
