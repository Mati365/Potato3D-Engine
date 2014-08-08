#version 400
layout(location = 0) out vec4 color;
in FragInfo {
	vec2		uv;
} frag;		
uniform	sampler2D			color_texture;
uniform	sampler2D			depth_texture;
uniform	float				blur;

const float	blur_array[9] = float[9](
		0.0204001988,
		0.0577929595,
		0.1215916882,
		0.1899858519,
		0.2204586031,
		0.1899858519,
		0.1215916882,
		0.0577929595,
		0.0204001988
);
vec2 pixelize(in float d, in vec2 pos) {
	return vec2(d * floor(pos.x / d), d * floor(pos.y / d));
}
void main(void) {
	float 	tilt_blur 	=	distance(vec2(0.5, 0.5), frag.uv) * blur;
	vec4	sum 		= 	vec4(0, 0, 0, 0);
	for(int i = -4;i <= 4;++i)
		sum +=	texture2D(color_texture, pixelize(0.008, vec2(frag.uv.x + i * tilt_blur, frag.uv.y))) * blur_array[i + 4];
	color = sum;
}
