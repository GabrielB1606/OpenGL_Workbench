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

    if( scene )
        ans = initFromScene(scene, filename);
    else
        std::cout << "Error parsing: " << filename << "\n";

    glBindVertexArray(0);

    return ans;
}

bool BasicMesh::initFromScene(const aiScene *scene, std::string filename)
{
    meshes.resize( scene->mNumMeshes );
    materials.resize( scene->mNumMaterials );

    unsigned int numVertices = 0;
    unsigned int numIndices = 0;

    for( unsigned int i = 0; i<meshes.size(); i++ ){
        meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        meshes[i].numIndices = scene->mMeshes[i]->mNumFaces *3;
        meshes[i].baseVertex = numVertices;
        meshes[i].baseIndex = numIndices;

        numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices += meshes[i].numIndices;
    }

    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    indices.reserve(numIndices);

    return false;
}
