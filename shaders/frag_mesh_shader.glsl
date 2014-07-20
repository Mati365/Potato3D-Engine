#version 400
in FragInfo {
	vec2		uv;
	vec3		normal;
	vec3		pos;
	vec3		cam;
	float		mtl;
} frag;
struct Material {	//	56B	+ 8B extra
	vec4				col[3];
	float				transparent;
	float				shine;
};
struct Light {
	vec3	pos;
	float	ambient_intensity;
	
	vec4	diffuse_col;
	float	diffuse_intensity;	
	
	vec4	specular_col;
	float	specular_intensity;
};

// ------------- UNIFORMY -----------------
layout(std140) uniform MaterialBlock {
	Material	material[4];
};
uniform	sampler2DArray		texture_pack;
uniform	vec4				col;

bool use_material =	frag.mtl >= 0;

#define	AMBIENT		0
#define	DIFFUSE		1
#define	SPECULAR	2
// ---------- TYLKO DLA TEXTUR -------------
#define	ALPHA		3
#define	BUMP		4
// -----------------------------------------

#define	GET_MATERIAL_UV_TEX(uv, type)	texture(texture_pack, vec3(uv , frag.mtl * (BUMP+1) + type))
#define	GET_MATERIAL_TEX(type)			GET_MATERIAL_UV_TEX(vec2(frag.uv.x, 1.0 - frag.uv.y), type)

Material 	MATERIAL 	= material[int(frag.mtl)];
Light		light 		= Light(
	vec3(0.0, 0.5, 0.0), // Pos
	1.0,				// Ambient intensity
	
	vec4(1.0, 1.0, 1.0, 1.0), // Diffuse col
	2.0,
	
	vec4(1.0, 1.0, 1.0, 1.0), // Specular col
	1.0
);

vec2 pixelize(in float d) {
	return vec2(d * floor(frag.uv.x / d), d * floor((1.0 - frag.uv.y) / d));
}
void calcLight(void) {
	vec3 normal	= normalize(frag.normal + 
			(use_material ? 
						normalize((GET_MATERIAL_TEX(BUMP).rgb * 2.0 - 1.0)) : 
						vec3(0,0,0)));
	// Diffuse
	float	distance		=	length(frag.pos - light.pos);
	vec3	light_normal	=	normalize(abs(frag.pos - light.pos));
	float	diffuse			=	max(dot(light_normal, normal), 0.0) 
									* (1.0 / (1.0 + (0.55 * distance * distance)));
	vec4	diffuse_col		=	use_material ? 
									(GET_MATERIAL_TEX(DIFFUSE) * MATERIAL.col[DIFFUSE]) : 
									col;
											
	// Specular
	float specular = 0.0;
	if(use_material) {
		vec3 reflect 	= 	normalize(2 * diffuse * normal - light_normal);
		vec3 viewDir 	=	normalize(abs(frag.cam - frag.pos));
		float aspect 	=	pow(max(dot(viewDir, reflect), 0.0), 4);
		
		specular = GET_MATERIAL_TEX(SPECULAR).g * 
							aspect * 
							light.specular_intensity;
	}
	
	// Całość
	vec3	col_out	=	
				MATERIAL.col[AMBIENT].rgb * light.ambient_intensity  + 
				(MATERIAL.col[SPECULAR] * specular * light.specular_col * light.specular_intensity).rgb;
	gl_FragColor += 
				vec4(col_out, 0.0) +
				diffuse_col * light.diffuse_col * light.diffuse_intensity * vec4(diffuse, diffuse, diffuse, 1.0);
	if(use_material)
		gl_FragColor.a *= MATERIAL.transparent;
}
void main(void) {
	if(!use_material)
		gl_FragColor =	col;
	calcLight();
}
