#version 400
out FragInfo {
	vec2		uv;
} frag;
struct Vertex {
	vec2	pos;
	vec2	uv;
};
const Vertex position[4] = Vertex[4](
    Vertex(vec2(-1.f,  1.f),	vec2(0.f, 1.f)),
    Vertex(vec2(-1.f,  -1.f),	vec2(0.f, 0.f)),
    Vertex(vec2(1.f,  1.f),		vec2(1.f, 1.f)),
    Vertex(vec2(1.f,  -1.f),	vec2(1.f, 0.f))
);
void main(void) {
	gl_Position 	= 	vec4(position[gl_VertexID].pos, 0.f, 1.f);
	frag.uv 		= 	position[gl_VertexID].uv;
}
