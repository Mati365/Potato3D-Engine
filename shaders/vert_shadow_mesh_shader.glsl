#version 400
layout(location = 0) in  vec4 	Position;

out vec3 pos;
struct Matrix {
	mat4	 	mvp;
	mat4		m;
};
uniform Matrix	matrix;

void main(void) {
    gl_Position = 	Position * matrix.mvp;
    pos			=	(Position * matrix.m).xyz;
}
