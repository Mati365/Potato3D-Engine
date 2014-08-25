#version 400
layout(location = 0) out float color;

in FragInfo {
	vec3		pos;
} frag;
vec3	light_pos = vec3(0.f, 0.f, 0.f);

void main(void) {
	color =  length(light_pos - frag.pos) / 4;
}
