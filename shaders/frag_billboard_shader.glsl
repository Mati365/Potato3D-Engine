#version 400
layout(location = 0) out vec4 color;
in FragInfo {
	vec2		uv;
} frag;

uniform	sampler2D			texture;
uniform	vec4				col;

void main(void) {
	color =	texture2D(texture, frag.uv) * col;
}
