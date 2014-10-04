#version 400
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
