#include <algorithm>

#include "Loaders.hpp"

namespace GL3Engine {
    /* MATERIAŁY */
    MTLloader::MTLloader()
            :
              ASCIIMeshLoader(
                      { { "newmtl", NAME },

                      { "Ka", AMBIENT_COL }, { "Kd", DIFFUSE_COL }, { "Ks", SPECULAR_COL },

                      { "map_Ka", AMBIENT_TEX }, { "map_Kd", DIFFUSE_TEX }, { "map_Ks", SPECULAR_TEX }, { "map_d", ALPHA_TEX }, { "map_bump", BUMP_TEX }, { "bump", BUMP_TEX },

                      { "Ns", SHINE }, { "d", TRANSPARENT }, { "Tr", TRANSPARENT } }) {
    }
    
    void MTLloader::onHeaderArgument(
            c_str file_dir, GLint active_header, LOADER_ITERATOR& it) {
        Material* material = this->mtl.empty() ? nullptr : this->mtl.back();
        if (active_header == NAME) {
            this->mtl.push_back(new Material);
            material = this->mtl.back();
            material->name = *it;
        }
        
#define DEFINE_3DVEC(header_type, mtl_type) \
               if(active_header==header_type) material->mtl_type = getVec3D(it);
#define DEFINE_F_1DVEC(header_type, mtl_type) \
               if(active_header==header_type) material->mtl_type = stringTo<GLfloat>(*it);
#define DEFINE_1DTEX(header_type, tex_type) \
               if(active_header==header_type) material->tex[tex_type] = file_dir + *it;
        
        // Parametry
        DEFINE_F_1DVEC(SHINE, shine);
        DEFINE_F_1DVEC(TRANSPARENT, transparent);
        
        // Kolory
        DEFINE_3DVEC(AMBIENT_COL, col[Material::AMBIENT]);
        DEFINE_3DVEC(DIFFUSE_COL, col[Material::DIFFUSE]);
        DEFINE_3DVEC(SPECULAR_COL, col[Material::SPECULAR]);
        
        // Tekstury
        DEFINE_1DTEX(AMBIENT_TEX, Material::AMBIENT);
        DEFINE_1DTEX(DIFFUSE_TEX, Material::DIFFUSE);
        DEFINE_1DTEX(SPECULAR_TEX, Material::SPECULAR);
        DEFINE_1DTEX(ALPHA_TEX, Material::ALPHA);
        DEFINE_1DTEX(BUMP_TEX, Material::BUMP);
    }
    void MTLloader::packTextures() {
        vector<string> textures;
        for (Material* material : mtl)
            for (string& str : material->tex)
                textures.push_back(str);
        
        TextureArray* array = new TextureArray(textures);
        for (Material* material : mtl)
            material->tex_array.reset(array);
    }
    
    /* SIATKI */
    OBJloader::OBJloader()
            :
              ASCIIMeshLoader(
                      { { "v", VERTEX }, { "vn", NORMAL }, { "vt", TEXTURE }, { "f", FACE }, { "mtllib", LOAD_MATERIAL }, { "usemtl", USE_MATERIAL } }) {
        mtl_loader.reset(new MTLloader);
    }
    
    void OBJloader::onNewHeader(GLint header, vector<string>&) {
        switch (header) {
            case FACE:
                if (polygon.empty())
                    return;
                finalizePolygon();
                break;
        }
    }
    void OBJloader::onHeaderArgument(
            c_str file_dir, GLint active_header, LOADER_ITERATOR& it) {
        switch (active_header) {
            // Plik MTL
            case LOAD_MATERIAL: {
                MATERIALS* ptr = (MATERIALS*) mtl_loader->load(file_dir + *it);
                if (ptr) {
                    materials.insert(materials.begin(), ptr->begin(),
                            ptr->end());
                    safeDelete(ptr, false);
                }
            }
                break;
            case USE_MATERIAL: {
                auto mat =
                        find_if(materials.begin(), materials.end(),
                                [&](Material*& a) {
                                    return a->name == *it;
                                });
                if (mat != materials.end())
                    used_material = mat - materials.begin();
                else
                    used_material = -1;
            }
                break;
                
                // Plik OBJ
            case VERTEX:
                indices.vertices.push_back(getVec3D(it));
                break;
            case NORMAL:
                indices.normals.push_back(getVec3D(it));
                break;
            case TEXTURE:
                indices.uv.push_back(getVec2D(it));
                break;
            case FACE:
                polygon.push_back(getVertex(it));
                break;
            default:
                break;
        }
    }
    
    Shape3D* OBJloader::createObject() {
        finalizePolygon();
        return new Shape3D(
                { &vertex_array[0], vertex_array.size() * sizeof(Vertex4f), GL_ARRAY_BUFFER, 0, GL_STATIC_DRAW },
                { nullptr, 0, GL_ELEMENT_ARRAY_BUFFER, 0, GL_STATIC_DRAW },
                { },
                materials);
    }
    void OBJloader::releaseMemory() {
        indices.clear();
        polygon.clear();
        vertex_array.clear();
        materials.clear();
        used_material = -1;
    }
    
    Vertex4f OBJloader::getVertex(LOADER_ITERATOR& iter) {
        Vertex4f v = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0, 0 }, -1 };
        string param = *iter;
        GLfloat args[3];
        
#define SHORT_COPY(size, arg_index, variable, destination_variable) \
                   arrayToRaw<GLfloat,size>(indices.variable[args[arg_index] - 1].toArray(), v.destination_variable)
        
        switch (count(param.begin(), param.end(), '/')) {
            // v1
            case 0:
                sscanf(param.c_str(), "%f", &args[0]);
                SHORT_COPY(3, 0, vertices, pos);
                break;
                // v1/vt1
            case 1:
                sscanf(param.c_str(), "%f/%f", &args[0], &args[1]);
                SHORT_COPY(3, 0, vertices, pos);
                SHORT_COPY(2, 1, uv, uv);
                break;
            case 2:
                if (param.find("//") != string::npos) {
                    // v1//vn1
                    sscanf(param.c_str(), "%f//%f", &args[0], &args[1]);
                    SHORT_COPY(3, 0, vertices, pos);
                    SHORT_COPY(3, 1, normals, normal);
                } else {
                    // v1/vt1/vn1
                    sscanf(param.c_str(), "%f/%f/%f", &args[0], &args[1],
                            &args[2]);
                    SHORT_COPY(3, 0, vertices, pos);
                    SHORT_COPY(2, 1, uv, uv);
                    SHORT_COPY(3, 2, normals, normal);
                }
                break;
        }
        // Pozostałe argumenty
        v.mtl = used_material;
        return v;
    }
}

