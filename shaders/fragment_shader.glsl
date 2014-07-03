#version 400
in vec2		frag_uv;
in float		dist;
in float		mtl;

struct Material {
	float				transparent;
	float				shine;
	vec4				col[4];
};
uniform	sampler2DArray	texture_pack;
uniform	vec4				col;
uniform	Material			material[4];

#define	AMBIENT	0
#define	DIFFUSE	1
#define	SPECULAR	2
// ---------- TYLKO DLA TEXTUR -------------
#define	ALPHA		3
#define	BUMP		4
// -----------------------------------------

#define	GET_MATERIAL_TEX(type)	texture(texture_pack, vec3(vec2(frag_uv.x, 1.0 - frag_uv.y), mtl * (BUMP+1) + ## type ##))
#define	MATERIAL						material[int(mtl)]

bool drawWithMaterial(void) {
	if(mtl < 0)
		return false;
	gl_FragColor = GET_MATERIAL_TEX(DIFFUSE) * MATERIAL.transparent;
	return true;
}
void main(void) {
	if(!drawWithMaterial())
		gl_FragColor =	col;
	gl_FragColor *= vec4(dist, dist, dist, 1);
}
