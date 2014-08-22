#version 400
layout(location = 0) out vec4 color;
in FragInfo {
	vec2		uv;
} frag;		
uniform	sampler2D			depth_texture;

void main(void) {
	color = texture2D(depth_texture, frag.uv);
}
