#version 400
layout(location = 0) out vec4 color;

#define MAX_LIGHTS		10
#define	MAX_MATERIALS	4

#define	AMBIENT		0
#define	DIFFUSE		1
#define	SPECULAR	2
// ---------- TYLKO DLA TEXTUR -------------
#define	ALPHA		3
#define	BUMP		4
// -----------------------------------------

in FragInfo {
	vec2		uv;
	float		mtl;
	mat3		surface2view;
	vec3		pos; // w model view
	mat4		v;
} frag;

// ------------- UNIFORMY -----------------
// std140
// tablica wielkosc * sizeof vec4
// chunk vec4
// Flagi mają dodane 1 bo enabled jest 1 + flaga
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
struct ShadowMap {
	samplerCube	point;
	sampler2D	direct;
};

// 48B + 12B + 4B = 64B
struct Material { // 136B size
	vec4				col[3]; 			// 48B
	float				tex_flag;			// 4B
	float				transparent; 		// 4B
	float				shine; 				// 4B
											// 4B extra
};
layout(std140) uniform MaterialBlock {
	Material	material[MAX_MATERIALS];
};

uniform	sampler2DArray		texture_pack;
uniform	ShadowMap			shadow_maps[MAX_LIGHTS];

#define	GET_MATERIAL_UV_TEX(uv, type)	texture(texture_pack, vec3(uv , frag.mtl * (BUMP+1) + type))
#define	GET_MATERIAL_TEX(type)			GET_MATERIAL_UV_TEX(vec2(frag.uv.x, 1.f - frag.uv.y), type)

#define	IS_MATERIAL_USED(material)		(int(MATERIAL.tex_flag) & (1 << material))
Material 	MATERIAL 	= 	material[int(frag.mtl)];

vec2 pixelize(in float d) {
	return vec2(d * floor(frag.uv.x / d), d * floor((1.f - frag.uv.y) / d));
}
void calcLight(in Light light, in int index) {
	vec3 normal;
	if(IS_MATERIAL_USED(BUMP))
		normal = normalize(GET_MATERIAL_TEX(BUMP).rgb * 2.f - 1.f);
	else
		normal = normalize(
					vec3(
						frag.surface2view[0][1], 
						frag.surface2view[1][1], 
						frag.surface2view[2][1]));
		
	// Diffuse
	vec3	light_normal,
			light_viewspace	=	(vec4(light.pos, 1.f) * frag.v).xyz;
	float	dist_prop, 
			shadow_factor	=	1.f;
		
	switch(light.type) {
		case POINT_LIGHT:
			light_normal 	= 	normalize(light_viewspace - frag.pos) * frag.surface2view;
			dist_prop 		= 	1.f / (1.f + (.5f * pow(length(abs(frag.pos - light_viewspace)), 1.f)));
		break;
		
		case DIRECT_LIGHT:
			light_normal	=	normalize(light.pos) * frag.surface2view;
			dist_prop		=	1.f;
		break;
	};
	
	float	diffuse			=	max(dot(light_normal, normal), 0.f) * dist_prop;
	vec3	diff			=	vec3(diffuse, diffuse, diffuse);
	
	// Specular
	if(IS_MATERIAL_USED(SPECULAR)) {
		vec3 	view_dir 		= 	normalize(frag.pos),
				reflection 		= 	reflect(light_normal, normal);
		float 	aspect 			=	pow(max(dot(reflection, view_dir), 0.f), 4.f);
		
		vec3 	spec_tex 	= 	GET_MATERIAL_TEX(SPECULAR).rgb;
		float	specular 	=	(spec_tex.r + spec_tex.g + spec_tex.b) * 
									aspect * 
									light.specular_intensity;
		if(dot(reflection, view_dir) > 0.f)
			color.rgb +=	(MATERIAL.col[SPECULAR] * 
								specular * 
								dist_prop *
								light.specular_col * 
								light.specular_intensity).rgb;
	}
							
	// Całość	
	color += vec4(MATERIAL.col[AMBIENT].rgb * light.ambient_intensity, 0.f);
	if(IS_MATERIAL_USED(DIFFUSE)) {
		vec4	diffuse_col	= GET_MATERIAL_TEX(DIFFUSE) * vec4(MATERIAL.col[DIFFUSE].rgb, 1.f);
		color += 
					diffuse_col * 
					light.diffuse_col * 
					light.diffuse_intensity * 
					vec4(diff, 1.f);
		color.a *= MATERIAL.transparent;
	} else
		color.rgb *= diff;
}
void main(void) {
	for(int i = 0;i < light_count;++i)
		calcLight(lights[i], i);
}
