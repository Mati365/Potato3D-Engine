#version 400
layout(location = 0) in  vec4 	Position;

out FragInfo {
	vec3		pos;
} frag;

struct Matrix {
	mat4	 	mvp;
	mat4		m;
};
uniform Matrix	matrix;

void main(void) {
    gl_Position = 	Position * matrix.mvp;
    frag.pos	=	(Position * matrix.m).xyz;
}
