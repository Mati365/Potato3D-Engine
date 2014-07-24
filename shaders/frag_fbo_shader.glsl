#version 400
layout(location = 0) out vec4 color;
in FragInfo {
	vec2		uv;
} frag;
uniform	sampler2D			texture;

vec2 pixelize(in float d) {
	return vec2(d * floor(frag.uv.x / d), d * floor(frag.uv.y / d));
}
void main(void) {
	color =	texture2D(texture, pixelize(0.008));
}
