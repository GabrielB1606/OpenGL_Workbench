#include "BasicMesh.h"

BasicMesh::~BasicMesh(){
    clear();
}

void BasicMesh::clear(){
    if (buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}

bool BasicMesh::loadMesh(std::string filename)
{

    // just in case lol
    clear();

    // create VAO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // generate buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);

    bool ans = false;
    
    scene = importer.ReadFile( filename.c_str(), ASSIMP_LOAD_FLAGS );

    return ans;
}