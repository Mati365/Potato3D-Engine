#version 400
layout(location = 0) in  vec4	Position;
layout(location = 1) in  vec2 	UV;

out FragInfo {
	vec2		uv;
} frag;

void main(void) {
	gl_Position 	= 	Position;
	frag.uv 		= 	UV;
}
