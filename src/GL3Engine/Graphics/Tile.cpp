#include "Texture.hpp"

namespace GL3Engine {
    namespace CoreMaterial {
        const GLushort Tile::quad_indices[6] = { 1, 0, 3, 1, 3, 2 };

        Tile::Tile(Texture* _tex, const CoreMatrix::Vec2i& _cells)
                :
                  tex(_tex),
                  cells(_cells),
                  cell_size( { 1.f / _cells.X(), 1.f / _cells.Y() }) {
            tokenize();
        }
        void Tile::tokenize() {
            uv.clear();
            for (GLfloat i = 0; i < 1.0; i += cell_size[1])
                for (GLfloat j = 0; j < 1.0; j += cell_size[0]) {
                    uv.push_back( {
                            { 0.f, cell_size[1], 0.f },
                            { j, i + cell_size[1] } });
                    uv.push_back( {
                            { cell_size[0], cell_size[1], 0.f },
                            { j + cell_size[0], i + cell_size[1] } });
                    uv.push_back( {
                            { cell_size[0], 0.f, 0.f },
                            { j + cell_size[0], i } });
                    uv.push_back( {
                            { 0.f, 0.f, 0.f },
                            { j, i } });
                }
        }
    }
}
