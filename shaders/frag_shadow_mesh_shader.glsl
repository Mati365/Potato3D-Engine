#version 400
layout(location = 0) out float color;

in vec3 		pos;
uniform	vec4	light_pos;

void main(void) {
	color = 1.f;
}
