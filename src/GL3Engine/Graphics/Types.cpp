#include "Types.hpp"

namespace GL3Engine {
    namespace CoreType {
        TYPE_IMPORT(CoreMatrix, Vec3);
        TYPE_IMPORT(CoreMatrix, Vec2);

        void calcTangents(VertexArray& vertices) {
            assert(!(vertices.size() % 3));
            for (GLuint i = 0; i < vertices.size(); i += 3)
                calcTangents(
                        std::array<Vertex4f*, 3> {
                                &vertices[i],
                                &vertices[i + 1],
                                &vertices[i + 2] });
        }
        void calcTangents(const std::array<Vertex4f*, 3>& tri) {
            Vec3 d_pos[] = {
                    // POS
                    (Vec3) tri[1]->pos - (Vec3) tri[0]->pos,
                    (Vec3) tri[2]->pos - (Vec3) tri[0]->pos
            };
            Vec2 d_uv[] = {
                    (Vec2) tri[1]->uv - (Vec2) tri[0]->uv,
                    (Vec2) tri[2]->uv - (Vec2) tri[0]->uv,
            };
            //         http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#top
            GLfloat r = 1.f
                    / (d_uv[0].X() * d_uv[1].Y() - d_uv[0].Y() * d_uv[1].X());
            Vec3 tangent =
                    (d_pos[0] * d_uv[1].Y() - d_pos[1] * d_uv[0].Y()) * r;
            Vec3 bitangent =
                    (d_pos[1] * d_uv[0].X() - d_pos[0] * d_uv[1].X()) * r;
            for (auto& v : tri)
                tangent.copyTo(v->tangent, 3);
            //            bitangent.copyTo(v->bitangent, 3);
        }
    }
}

