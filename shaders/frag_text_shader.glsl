#version 400
in FragInfo {
	vec2		uv;
} frag;

uniform	sampler2D			texture;
uniform	vec4				col;

void main(void) {
	gl_FragColor =	texture2D(texture, frag.uv) * col;
}
