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

    for (size_t i = 0; i < meshes.size(); i++){
        const aiMesh* aiMeshPointer = scene->mMeshes[i];
        initSingleMesh(i, aiMeshPointer);
    }
    


    return false;
}

bool BasicMesh::initSingleMesh(unsigned int meshIndex, const aiMesh *aiMeshPointer){

    // get the information of each vertex
    for( size_t i=0; i<aiMeshPointer->mNumVertices; i++ ){
        
        const aiVector3D &pos = aiMeshPointer->mVertices[i];
        positions.push_back( glm::vec3(pos.x, pos.y, pos.z) );

        if( aiMeshPointer->mNormals ){
            const aiVector3D &normal = aiMeshPointer->mNormals[i];
            normals.push_back( glm::vec3(normal.x, normal.y, normal.z) );
        }else{
            normals.push_back( glm::vec3(0.f, 0.f, 0.1f) );
        }

        if( aiMeshPointer->HasTextureCoords(0) ){
            const aiVector3D &texCoord = aiMeshPointer->mTextureCoords[0][i];
            texCoords.push_back( glm::vec2(texCoord.x, texCoord.y) );
        }else{
            texCoords.push_back( glm::vec2(0.f, 0.f) );
        }

    }

    // index buffer
    for (size_t i = 0; i < aiMeshPointer->mNumFaces; i++){
        const aiFace &face = aiMeshPointer->mFaces[i];
        indices.push_back( face.mIndices[0] );
        indices.push_back( face.mIndices[1] );
        indices.push_back( face.mIndices[2] );
    }
    

    return true;
}

bool BasicMesh::initMaterials(const aiScene* scene, std::string textureDir){
    bool ans = true;

    for (size_t i = 0; i < scene->mNumMaterials; i++){
        const aiMaterial* mat = scene->mMaterials[i];

    }
    
    // load colors

    return ans;
}
