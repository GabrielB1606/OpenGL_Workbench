#ifndef BASICMESH_H
#define BASICMESH_H

#include "headers.h"

class BasicMesh{
private:
    
    enum BUFFER_TYPE{
        INDEX_BUFFER = 0,
        POS_VB       = 1,
        TEXCOORD_VB  = 2,
        NORMAL_VB    = 3,
        WVP_MAT_VB   = 4,  // required only for instancing
        WORLD_MAT_VB = 5,  // required only for instancing
        NUM_BUFFERS  = 6
    };

    GLuint VAO;
    GLuint buffers[NUM_BUFFERS] = {0};

    // should have a vector of materials
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<GLuint> indices;

    Assimp::Importer importer;
    const aiScene* scene;

public:
    BasicMesh(/* args */){}
    ~BasicMesh();

    void clear();
    bool loadMesh(std::string filename);
};


#endif