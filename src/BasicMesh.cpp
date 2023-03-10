#include "BasicMesh.h"

#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2

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
    
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string textureDir;

    if( slashIndex == std::string::npos )
        textureDir = ".";
    else if(slashIndex == 0)
        textureDir = "/";
    else
        textureDir = filename.substr(0, slashIndex);
    
    initMaterials(scene, textureDir);

    return 1;
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
        loadColors(mat, i);
    }

    return ans;
}

void BasicMesh::loadColors(const aiMaterial *mat, int index){
    aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
    glm::vec3 AllOnes(1.0f, 1.0f, 1.0f);

    int ShadingModel = 0;
    if (mat->Get(AI_MATKEY_SHADING_MODEL, ShadingModel) == AI_SUCCESS) {
        printf("Shading model %d\n", ShadingModel);
    }

    if (mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS) {
        printf("Loaded ambient color [%f %f %f]\n", ambientColor.r, ambientColor.g, ambientColor.b);
        materials[index].ambientColor.r = ambientColor.r;
        materials[index].ambientColor.g = ambientColor.g;
        materials[index].ambientColor.b = ambientColor.b;
    } else {
        materials[index].ambientColor = AllOnes;
    }

    aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);

    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) {
        printf("Loaded diffuse color [%f %f %f]\n", diffuseColor.r, diffuseColor.g, diffuseColor.b);
        materials[index].diffuseColor.r = diffuseColor.r;
        materials[index].diffuseColor.g = diffuseColor.g;
        materials[index].diffuseColor.b = diffuseColor.b;
    }

    aiColor3D specularColor(0.0f, 0.0f, 0.0f);

    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
        printf("Loaded specular color [%f %f %f]\n", specularColor.r, specularColor.g, specularColor.b);
        materials[index].specularColor.r = specularColor.r;
        materials[index].specularColor.g = specularColor.g;
        materials[index].specularColor.b = specularColor.b;
    }
}

void BasicMesh::populateBuffers(){
    glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void BasicMesh::render(){
    glBindVertexArray(this->VAO);

    for (size_t i = 0; i < meshes.size(); i++){
        size_t matIndex = meshes[i].materialIndex;

        // assert( matIndex < textures.size() );

        // if( textures[matIndex] )
        //     textures->bind(COLOR_TEXTURE_UNIT);

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void *)(sizeof(unsigned int) * meshes[i].baseIndex ),
            meshes[i].baseVertex
        );

    }

    glBindVertexArray(0);
    
}
