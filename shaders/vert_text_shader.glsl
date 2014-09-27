#version 400
layout(location = 0) in  vec3 	Position;
layout(location = 1) in  vec2 	UV;

out FragInfo {
	vec2		uv;
} frag;
struct Matrix {
	mat4	 	mvp;
};
uniform Matrix	matrix;

void main(void) {
	gl_Position 	= 	vec4(Position, 1.f) * matrix.mvp;
	frag.uv 		= 	UV;
}
