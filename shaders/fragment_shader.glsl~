#version 400
in 	vec2		frag_uv;
in	vec3		frag_normal;
in	vec3		frag_pos;
in 	float		mtl;

struct Material {
	float				transparent;
	float				shine;
	vec4				col[4];
};
uniform	Material			material[4];
uniform	sampler2DArray		texture_pack;
uniform	vec4				col;

bool 		use_material	=	mtl >= 0;

#define	AMBIENT		0
#define	DIFFUSE		1
#define	SPECULAR	2
// ---------- TYLKO DLA TEXTUR -------------
#define	ALPHA		3
#define	BUMP		4
// -----------------------------------------

#define	GET_MATERIAL_UV_TEX(uv, type)	texture(texture_pack, vec3(uv , mtl * (BUMP+1) + type))
#define	GET_MATERIAL_TEX(type)			GET_MATERIAL_UV_TEX(vec2(frag_uv.x, 1.0 - frag_uv.y), type)
#define	MATERIAL						material[int(mtl)]

struct Light {
	vec3	pos;

	vec3	diffuse_col;
	float	diffuse_intensity;	
	
	vec3	specular_col;
};
Light	light = Light(
	vec3(1.0, 0.5, 1.0), // Pos
	vec3(1.0, 1.0, 1.0), // Diffuse col
	1.95,
	vec3(1.0, 1.0, 1.0) // Specular col
);

vec2 pixelize(in float d) {
	return vec2(d * floor(frag_uv.x / d), d * floor((1.0 - frag_uv.y) / d));
}
void calcDiffuseLight(void) {
	vec3	normal			=	normalize(frag_normal + 
			(use_material ? 
						normalize((GET_MATERIAL_TEX(BUMP).rgb * 2.0 - 1.0)) : 
						vec3(0,0,0)));
	
	float	distance		=	length(light.pos - frag_pos);
	vec3	light_normal	=	normalize(light.pos - frag_pos);
	float	diffuse			=	max(dot(light_normal, normal), 0.0) * (1.0 / (1.0 + (0.25 * distance * distance)));
	gl_FragColor.rgb 		*= 	(light.diffuse_col + MATERIAL.shine / 1000.0) * diffuse * light.diffuse_intensity;
}
bool drawMaterial(void) {
	if(!use_material)
		return false;
	gl_FragColor = GET_MATERIAL_TEX(DIFFUSE) * MATERIAL.transparent;
	return true;
}
void main(void) {
	if(!drawMaterial())
		gl_FragColor =	col;
	calcDiffuseLight();
}
