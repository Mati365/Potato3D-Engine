#version 400
layout(location = 0) out float color;

in FragInfo {
	vec3		pos;
} frag;

void main(void) {
	color =  gl_FragCoord.z;
}
