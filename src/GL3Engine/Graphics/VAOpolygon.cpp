#include "Mesh.hpp"

#include "Effects.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;
    
    GLint genGLBuffer(const GL_BUFFER_DATA& data, bool bind) {
        GLuint buffer = 0;
        
        glGenBuffers(1, &buffer);
        glBindBuffer(data.type, buffer);
        glBufferData(data.type, data.len, data.data, data.vbo_draw_type);
        if (!bind)
            glBindBuffer(data.type, 0);
        
        return buffer;
    }
    
    template<typename T>
    void VAOpolygon<T>::create(
            const GL_BUFFER_DATA& vertices, const GL_BUFFER_DATA& indices) {
        // Generowanie osobnego VBO dla obiektu
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        // Generowanie bufora indeksow
        this->indices = genGLBuffer(indices, true);
        this->indices_count = indices.len / sizeof(GLuint);
        
        // Generowanie bufora wierzcholkow
        this->vbo = genGLBuffer(vertices, true);
        this->vertices_count = vertices.len / sizeof(T);
        
        // Bardzo zły pomysł ale działa!
        if (typeid(T) == typeid(Vertex4f)) {
            VERTEX_ATTR_PTR(0, 3, 0, T); // Vertex
            VERTEX_ATTR_PTR(1, 3, 3, T); // Normals
            VERTEX_ATTR_PTR(2, 2, 6, T); // UVs
            VERTEX_ATTR_PTR(3, 1, 8, T); // MTL
        } else {
            VERTEX_ATTR_PTR(0, 3, 0, T); // Vertex
            VERTEX_ATTR_PTR(1, 2, 3, T); // UV
        }
        
        glBindVertexArray(0);
    }
    template<typename T>
    void VAOpolygon<T>::changeData(
            const GL_BUFFER_DATA& _vertices, const GL_BUFFER_DATA& _indices) {
        glBindVertexArray(vao);
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _indices.offset,
                    _indices.len, _indices.data);
            
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, _vertices.offset, _vertices.len,
                    _vertices.data);
        }
        glBindVertexArray(0);
        
        vertices_count = (_vertices.offset + _vertices.len) / sizeof(T);
        indices_count = (_indices.offset + _indices.len) / sizeof(GLuint);
    }
    template<typename T>
    VAOpolygon<T>::~VAOpolygon() {
        while (!materials.empty()) {
            safeDelete(materials.back(), false);
            materials.pop_back();
        }
    }
    
    template class VAOpolygon<Vertex4f> ;
    template class VAOpolygon<Vertex2f> ;
}
