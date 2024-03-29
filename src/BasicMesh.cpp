#include "BasicMesh.h"

#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2

BasicMesh::~BasicMesh(){
    clear();

    if( surrounding != nullptr )
        delete surrounding;

}

BasicMesh::BasicMesh(){

    translation = std::shared_ptr<glm::vec3>(new glm::vec3(0.f));
    rotation    = std::shared_ptr<glm::mat4>(new glm::mat4(1.f));
    scale       = std::shared_ptr<glm::vec3>(new glm::vec3(1.f));

    calculateModelMatrix();
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

bool BasicMesh::loadMesh(std::string filename){

    // just in case lol
    clear();

    // create VAO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // generate buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);

    bool ans = false;
    
    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
    // Assimp::DefaultLogger::get()->attachStream(new Assimp::DefaultLogger::FileLogStream("assimp_log.txt", "w"));
    

    scene = importer.ReadFile( filename.c_str(), ASSIMP_LOAD_FLAGS );

    if( scene!=nullptr )
        ans = initFromScene(scene, filename);
    else{
        std::cout << "Error parsing: " << filename << "\n";
        std::string error = importer.GetErrorString();
        std::cerr << "Error importing file: " << error << std::endl;    
    }

    Assimp::DefaultLogger::kill();

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

        meshes[i].name = scene->mMeshes[i]->mName.length > 0? scene->mMeshes[i]->mName.C_Str() : "Mesh " + std::to_string(i);

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
    
    // this is not cross platform
    std::string::size_type slashIndex = filename.find_last_of("\\");

    if( slashIndex == std::string::npos )
        this->filename = filename.substr(slashIndex);
    else
        this->filename = filename;

    std::string textureDir;

    if( slashIndex == std::string::npos )
        textureDir = ".";
    else if(slashIndex == 0)
        textureDir = "\\";
    else
        textureDir = filename.substr(0, slashIndex);
    
    initMaterials(scene, textureDir);

    populateBuffers();

    return 1;
}

bool BasicMesh::initSingleMesh(size_t meshIndex, const aiMesh *aiMeshPointer){

    // get the information of each vertex
    for( size_t i=0; i<aiMeshPointer->mNumVertices; i++ ){
        
        const aiVector3D &pos = aiMeshPointer->mVertices[i];
        positions.push_back( glm::vec3(pos.x, pos.y, pos.z) );
        // std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";

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
        loadTextures(textureDir, mat, i);
        loadColors(mat, i);
    }

    return ans;
}

void BasicMesh::loadColors(const aiMaterial *mat, size_t index){
    aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
    glm::vec3 AllOnes(1.0f, 1.0f, 1.0f);

    int ShadingModel = 0;
    if (mat->Get(AI_MATKEY_SHADING_MODEL, ShadingModel) == AI_SUCCESS) {
        // printf("Shading model %d\n", ShadingModel);
    }

    if (mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS) {
        // printf("Loaded ambient color [%f %f %f]\n", ambientColor.r, ambientColor.g, ambientColor.b);
        materials[index].ambientColor.x = ambientColor.r;
        materials[index].ambientColor.y = ambientColor.g;
        materials[index].ambientColor.z = ambientColor.b;
    } else {
        materials[index].ambientColor = AllOnes;
    }

    aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);

    if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS) {
        // printf("Loaded diffuse color [%f %f %f]\n", diffuseColor.r, diffuseColor.g, diffuseColor.b);
        materials[index].diffuseColor.x = diffuseColor.r;
        materials[index].diffuseColor.y = diffuseColor.g;
        materials[index].diffuseColor.z = diffuseColor.b;
    }

    aiColor3D specularColor(0.0f, 0.0f, 0.0f);

    if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
        // printf("Loaded specular color [%f %f %f]\n", specularColor.r, specularColor.g, specularColor.b);
        materials[index].specularColor.x = specularColor.r;
        materials[index].specularColor.y = specularColor.g;
        materials[index].specularColor.z = specularColor.b;
    }

    if( mat->Get(AI_MATKEY_SHININESS, materials[index].shininess ) != AI_SUCCESS )
        materials[index].shininess = -1.f;

}

void BasicMesh::loadTextures(std::string dir, const aiMaterial *mat, size_t index){

    aiString path;
    std::string p;

    if( mat->GetTextureCount( aiTextureType_DIFFUSE ) > 0 ){

        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            const aiTexture* paiTexture = scene->GetEmbeddedTexture(path.C_Str());

            p = path.data;
            if( p.substr(0,2) == ".\\" )
                p = p.substr(2, p.size()-2);
            
            p = dir + "/" + p;

            materials[index].textures[Texture::TYPE::DIFFUSE] = new Texture(p, GL_TEXTURE_2D);

        }

    }

    if( mat->GetTextureCount( aiTextureType_SPECULAR ) > 0 ){

        if (mat->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            const aiTexture* paiTexture = scene->GetEmbeddedTexture(path.C_Str());

            p = path.data;
            if( p.substr(0,2) == ".\\" )
                p = p.substr(2, p.size()-2);
            
            p = dir + "/" + p;

            materials[index].textures[Texture::TYPE::SPECULAR] = new Texture(p, GL_TEXTURE_2D);

        }

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

void BasicMesh::render(ShaderProgram *shader){

    sendUniforms(shader);

    shader->use();

    glBindVertexArray(this->VAO);

    for (size_t i = 0; i < meshes.size(); i++){

        materials[ meshes[i].materialIndex ].sendUniforms(shader);

        shader->use();

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int)*meshes[i].baseIndex),
            meshes[i].baseVertex
        );

        materials[ meshes[i].materialIndex ].unbind();
        
        //  for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << shader->getID() << ") OpenGL error: " << error << std::endl;
        }

    }

    glBindVertexArray(0);

    shader->stopUsing();
}

void BasicMesh::render(ShaderProgram *shader, glm::mat4 projViewMatrix){
    
    sendUniforms(shader);
    shader->setMat4fv( projViewMatrix * this->modelMatrix , "ProjViewModelMatrix", false);
    shader->set1f( this->eta, "eta" );

    shader->use();

    glBindVertexArray(this->VAO);

    for (size_t i = 0; i < meshes.size(); i++){

        materials[ meshes[i].materialIndex ].sendUniforms(shader);

        shader->use();

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int)*meshes[i].baseIndex),
            meshes[i].baseVertex
        );

        materials[ meshes[i].materialIndex ].unbind();
        
        //  for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << shader->getID() << ") OpenGL error: " << error << std::endl;
        }

    }

    glBindVertexArray(0);

    shader->stopUsing();
}

void BasicMesh::renderRefractive(ShaderProgram *shader, glm::mat4 projViewMatrix, SceneFBO *sceneTexture){
    
    sendUniforms(shader);
    shader->setMat4fv( projViewMatrix * this->modelMatrix , "ProjViewModelMatrix", false);
    shader->set1f( this->eta, "eta" );

    shader->use();

    sceneTexture->bindRead(GL_TEXTURE0);

    glBindVertexArray(this->VAO);

    for (size_t i = 0; i < meshes.size(); i++){

        materials[ meshes[i].materialIndex ].sendUniforms(shader);

        shader->use();

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int)*meshes[i].baseIndex),
            meshes[i].baseVertex
        );

        materials[ meshes[i].materialIndex ].unbind();
        
        //  for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << shader->getID() << ") OpenGL error: " << error << std::endl;
        }

    }

    glBindVertexArray(0);

    shader->stopUsing();
}

void BasicMesh::renderRefractive(ShaderProgram *shader, glm::mat4 projViewMatrix){
    
    sendUniforms(shader);
    shader->setMat4fv( projViewMatrix * this->modelMatrix , "ProjViewModelMatrix", false);
    shader->set1f( this->eta, "eta" );

    shader->use();

    if(this->surrounding == nullptr)
        return;

    this->surrounding->bindRead(GL_TEXTURE5);

    glBindVertexArray(this->VAO);

    for (size_t i = 0; i < meshes.size(); i++){

        materials[ meshes[i].materialIndex ].sendUniforms(shader);

        shader->use();

        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int)*meshes[i].baseIndex),
            meshes[i].baseVertex
        );

        materials[ meshes[i].materialIndex ].unbind();
        
        //  for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << shader->getID() << ") OpenGL error: " << error << std::endl;
        }

    }

    glBindVertexArray(0);

    shader->stopUsing();
}

void BasicMesh::sendUniforms(ShaderProgram *shader){

    shader->setMat4fv(this->modelMatrix, "ModelMatrix", false);
    shader->setMat4fv(this->invModelMatrix, "InverseModelMatrix", false);
    shader->set1i(this->isShadowReceiver(), "isShadowReceiver");

}

void BasicMesh::renderSurroundings(std::vector<BasicMesh *> meshes, Skybox *sky, ShaderProgram* shader){

    glm::vec3 cubeMapTargets[6] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    };

    glm::vec3 cubeMapUps[6] = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    if( this->surrounding == nullptr ){
        this->surrounding = new CubeMapFBO();
        this->surrounding->init(SURROUNDING_MAP_SIZE);
    }

    glm::mat4 projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 1000.f);
    glm::mat4 viewMatrix;
    glm::mat4 projView;

    for (size_t i = 0; i < 6; i++){
        this->surrounding->bindWrite( GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum)i );

        viewMatrix = glm::lookAt( *this->translation, *this->translation + cubeMapTargets[i], cubeMapUps[i] );
        projView = projection * viewMatrix;

        // Clear the color and depth buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(BasicMesh* mesh : meshes)
            mesh->render(shader, projView);
        
        // if( !floor->isReflective() )
        //     floor->render(shader, projView);
        // else
        //     floor->mirror(shader, meshes, projView, viewMatrix, sky);

        sky->render(viewMatrix);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void BasicMesh::calculateModelMatrix(){

    this->modelMatrix = glm::mat4(1.f);
    this->modelMatrix = glm::translate( this->modelMatrix, *this->translation.get() );
    this->modelMatrix = this->modelMatrix * (*this->rotation.get());
    this->modelMatrix = glm::scale(this->modelMatrix, *this->scale.get());
    this->invModelMatrix = glm::inverse(this->modelMatrix);

}

void BasicMesh::translate(glm::vec3 vec)
{
    (*this->translation) += vec;
    this->calculateModelMatrix();
}

void BasicMesh::rotate(glm::vec3 vec){

    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(vec.x), glm::vec3(1.f, 0.f, 0.f) ) * (*this->rotation);
    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(vec.y), glm::vec3(0.f, 1.f, 0.f) ) * (*this->rotation);
    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(vec.z), glm::vec3(0.f, 0.f, 1.f) ) * (*this->rotation);

    this->calculateModelMatrix();

}

void BasicMesh::scaleUp(glm::vec3 vec){

    (*this->scale) += vec;

    this->calculateModelMatrix();

}

glm::vec3 BasicMesh::getPosition(){
    return *this->translation;
}

glm::vec3 BasicMesh::getScale(){
    return *this->scale;
}

std::shared_ptr<glm::vec3> BasicMesh::getTranslationReference(){
    return this->translation;
}

std::shared_ptr<glm::vec3> BasicMesh::getScaleReference(){
    return this->scale;
}

std::string BasicMesh::getSubMeshName(size_t index){

    if(index >= this->meshes.size())
        return "";

    return this->meshes[index].name ;
}

std::string BasicMesh::getName(){
    return this->filename;
}

size_t BasicMesh::getSubMeshesSize(){
    return this->meshes.size();
}

bool BasicMesh::isShadowCaster(){
    return this->shadowCaster;
}

bool BasicMesh::isShadowReceiver(){
    return this->shadowReceiver;
}

bool BasicMesh::isRefractive(){
    return this->refractive;
}

bool *BasicMesh::getRefractiveReference(){
    return &this->refractive;
}

bool *BasicMesh::getShadowCasterReference(){
    return &this->shadowCaster;
}

bool *BasicMesh::getShadowReceiverReference(){
    return &this->shadowReceiver;
}

float *BasicMesh::getEtaReference(){
    return &this->eta;
}

BasicMeshEntry BasicMesh::getMeshEntry(size_t index){
    return this->meshes[index];
}

GLuint BasicMesh::getVAO(){
    return this->VAO;
}

void *BasicMesh::getIndices_ptr(unsigned int offset){
    return (void*)(sizeof(unsigned int)*offset);
}

void BasicMesh::setShadowCaster(bool b){
    this->shadowCaster = b;
}

void BasicMesh::setShadowReceiver(bool b){
    this->shadowReceiver = b;
}

void BasicMesh::attatchPosition(std::shared_ptr<glm::vec3> position){
    this->translation.reset();
    this->translation = position;
}
