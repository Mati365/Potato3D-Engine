#version 400
in vec2		frag_uv;
in float		dist;
in float		mtl;

struct Material {
	float			transparent;
	float			shine;
	vec4			col[4];
	sampler2D	dtex;
};
uniform	vec4			col;
uniform	Material		material[4];

void main(void) {
	vec4 _col = vec4(col.r, col.g, col.b, 1);
	if(mtl >= 0)
		gl_FragColor =	texture2D(material[int(mtl)].dtex, frag_uv);
	else
		gl_FragColor =	_col;
	gl_FragColor *= vec4(dist, dist, dist, 1);
}
