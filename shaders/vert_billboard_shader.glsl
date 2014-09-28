#version 400
layout(location = 0) in  vec3 	Position;
layout(location = 1) in  vec2 	UV;

out FragInfo {
	vec2		uv;
} frag;
struct Matrix {
	mat4	 	mv;
	mat4		proj;
	vec2		scale;
};
uniform Matrix	matrix;

void main(void) {
	gl_Position 	= 	(
							vec4(.0f, .0f, .0f, 1.f) * matrix.mv + 
							vec4(Position.xy, 0.f, 0.f) * vec4(matrix.scale.x, matrix.scale.y, 1.f, 1.f)
						) * matrix.proj;
	frag.uv 		= 	UV;
}
