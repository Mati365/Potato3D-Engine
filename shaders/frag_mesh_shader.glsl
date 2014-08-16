#version 400

#define	AMBIENT		0
#define	DIFFUSE		1
#define	SPECULAR	2
// ---------- TYLKO DLA TEXTUR -------------
#define	ALPHA		3
#define	BUMP		4
// -----------------------------------------

in FragInfo {
	vec2		uv;
	vec3		normal;
	vec3		pos;
	vec3		cam;
	float		mtl;
} frag;

// ------------- UNIFORMY -----------------
// std140
// tablica wielkosc * sizeof vec4
// chunk vec4
// Flagi mają dodane 1 bo enabled jest 1 + flaga
#define MAX_LIGHTS		10
#define	POINT_LIGHT		5
#define	DIRECT_LIGHT	9
struct Light {
	vec3	pos;							//	16B
	vec4	specular_col;					//	16B
	vec4	diffuse_col; 					// 	16B
	
	float	ambient_intensity;				// 	4B
	float	diffuse_intensity;	 			// 	4B
	float	specular_intensity;  			// 	4B
	
	int		type;							//	4B extra
};
layout(std140) uniform LightBlock {
	Light		lights[MAX_LIGHTS];
	float		light_count;
};

// 3 bloki po 136B+8B extra i 4 block 136B
struct Material { // 136B size
	vec4				col[3]; 			// 48B
	bool				tex_flag[BUMP + 1]; // 80B
	float				transparent; 		// 4B
	float				shine; 				// 4B
											// 8B extra
};
layout(std140) uniform MaterialBlock {
	Material	material[4];
};

uniform	sampler2DArray		texture_pack;
uniform	vec4				col;

#define	GET_MATERIAL_UV_TEX(uv, type)	texture(texture_pack, vec3(uv , frag.mtl * (BUMP+1) + type))
#define	GET_MATERIAL_TEX(type)			GET_MATERIAL_UV_TEX(vec2(frag.uv.x, 1.f - frag.uv.y), type)

Material 	MATERIAL 	= 	material[int(frag.mtl)];

vec2 pixelize(in float d) {
	return vec2(d * floor(frag.uv.x / d), d * floor((1.f - frag.uv.y) / d));
}
void calcLight(in Light light) {
	vec3 normal;
	if(MATERIAL.tex_flag[BUMP])
		normal = normalize(GET_MATERIAL_TEX(BUMP).rgb * 2.f - 1.f);
	else
		normal = normalize(frag.normal);
	
	// Diffuse
	vec3	light_normal; 
	float	dist_prop;
	switch(light.type) {
		case POINT_LIGHT:
			light_normal	= 	normalize(abs(frag.pos - light.pos));
			dist_prop		=  	1.f / (1.f + (.5 * pow(length(frag.pos - light.pos), 1.f)));
		break;
		case DIRECT_LIGHT:
			light_normal	=	frag.pos;
			dist_prop		=	1.f;
		break;
	};
	
	float	diffuse			=	max(dot(light_normal, normal), 0.f) 
									* dist_prop;
	vec4	diff			=	vec4(diffuse, diffuse, diffuse, 1.f);
												
	// Specular
	if(MATERIAL.tex_flag[SPECULAR]) {
		vec3 	reflect 	= 	normalize(2.f * diffuse * normal - light_normal);
		vec3 	viewDir 	=	normalize(abs(frag.cam - frag.pos));
		float 	aspect 		=	pow(max(dot(viewDir, reflect), 0.f), 2.f);
		float 	specular 	= 	0.f;
		vec3 	spec_tex 	= 	GET_MATERIAL_TEX(SPECULAR).rgb;
		specular = (spec_tex.r + spec_tex.g + spec_tex.b) * 
							aspect * 
							light.specular_intensity;
		gl_FragColor +=	
					vec4(
						(MATERIAL.col[SPECULAR] * 
						specular * 
						light.specular_col * 
						light.specular_intensity).rgb, 0.f);
	}
	
	// Całość
	if(MATERIAL.tex_flag[AMBIENT])	
		gl_FragColor += vec4(MATERIAL.col[AMBIENT].rgb * light.ambient_intensity, 0.f);
	if(MATERIAL.tex_flag[DIFFUSE]) {
		vec4	diffuse_col	= GET_MATERIAL_TEX(DIFFUSE) * MATERIAL.col[DIFFUSE];
		diffuse_col.a = 1.f;
		gl_FragColor += 
					diffuse_col * 
					light.diffuse_col * 
					light.diffuse_intensity * 
					diff;
		gl_FragColor.a *= MATERIAL.transparent;
	} else
		gl_FragColor =	col * diff;
}
void main(void) {
	for(int i = 0;i < light_count;++i)
		calcLight(lights[i]);
}
