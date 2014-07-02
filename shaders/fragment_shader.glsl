#version 400
in vec2		frag_uv;
in float		dist;
in float		mtl;

struct Material {
	float			transparent;
	float			shine;
	vec4			col[4];
	
	sampler2D	diffuse_tex;
	sampler2D	bump_tex;
};
uniform	vec4			col;
uniform	Material		material[4];

bool drawWithMaterial(void) {
	if(mtl < 0)
		return false;
	// Diffuse	
	gl_FragColor =	texture2D(material[int(mtl)].diffuse_tex, frag_uv);
	return true;
}
void main(void) {
	if(!drawWithMaterial())
		gl_FragColor =	vec4(col.r, col.g, col.b, 1);
	gl_FragColor *= vec4(dist, dist, dist, 1);
}
