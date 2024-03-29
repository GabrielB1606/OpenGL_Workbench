#include "World.h"

World::World(float fov, float width, float height, float nearClippingPlane, float farClippingPlane){

    this->fov = fov;
    this->width = width;
    this->height = height;
    this->nearPlane = nearClippingPlane;
    this->farPlane = farClippingPlane;

    this->perspectiveMatrix = glm::perspective(glm::radians(fov), width/height, nearPlane, farPlane);
    this->perspectiveUpdated = true;

}

World::~World(){

    for(size_t i = 0; i < meshes.size(); i++)
        delete meshes[i];
    
    for(size_t i = 0; i < lights.size(); i++)
        delete lights[i];

    if(skybox != nullptr)
        delete skybox;

    if(floor != nullptr)
        delete floor;

}

float World::getWidth(){
    return this->width;
}

float World::getHeight(){
    return this->height;
}

void World::setFOV(float fov){

    this->perspectiveUpdated = false;

    this->fov = fov;
}

void World::setAspectRatio(float width, float height){

    perspectiveUpdated = false;

    this->width = width;
    this->height = height;

}

void World::createSkybox(ShaderProgram *skyboxProgram, std::string directory, std::string format){

    if( this->skybox != nullptr )
        delete this->skybox;
    
    this->skybox = new Skybox(skyboxProgram, directory, format);

}

void World::changeSkybox(std::string directory, std::string format){

    if( this->skybox != nullptr )
        this->skybox->loadCubeTextures(directory, format);

}

void World::createFloor(){

    if( this->floor == nullptr )
        delete this->floor;
    
    this->floor = new Plane(100, 100, glm::vec3(-50.f, -1.75f, -50.f));

}

glm::mat4 World::getPerspectiveMatrix(){

    if( !this->perspectiveUpdated ){
        this->perspectiveMatrix = glm::perspective(glm::radians(fov), width/height, nearPlane, farPlane);
        this->perspectiveUpdated = true;
    }

    return this->perspectiveMatrix;
}

std::vector<BasicMesh*> World::getMeshes(){
    return this->meshes;
}

std::vector<BasicMesh *> *World::getMeshesVectorReference()
{
    return &this->meshes;
}

std::vector<Light *> World::getLights(){
    return lights;
}

Plane *World::getFloor(){
    return this->floor;
}

BasicMesh *World::getMesh(size_t index){

    if( index >= this->meshes.size() )
        return nullptr;
    
    return this->meshes[index];

}

Light *World::getLight(size_t index){
    
    if( index >= this->lights.size() )
        return nullptr;

    return this->lights[index];

}

size_t World::getMeshesSize(){
    return this->meshes.size();
}

void World::loadMesh(std::string path){

    meshes.push_back( new BasicMesh() );
    meshes[ meshes.size()-1 ]->loadMesh(path);

}

void World::addLight(Light *l){
    lights.push_back(l);    
}

void World::removeMesh(size_t index){

    delete this->meshes[index];
    this->meshes.erase( this->meshes.begin()+index );

}

void World::removeLight(size_t index){

    delete this->lights[index];
    this->lights.erase( this->lights.begin()+index );

}

void World::sendUniforms(ShaderProgram *shader){

    for(size_t i =0; i<lights.size(); i++)
        lights[i]->sendUniforms(shader, i);

    shader->set1i( (GLint)lights.size() , "nLights");

}

std::shared_ptr<SceneFBO> World::renderSceneFBO(ShaderProgram *shader, ViewCamera *cam){

    std::shared_ptr<SceneFBO> sceneFrame = std::shared_ptr<SceneFBO>(new SceneFBO());

    sceneFrame->init(this->width, this->height);
    sceneFrame->bindWrite();

    this->renderSkybox(cam->getViewMatrix());
    this->renderFloor(shader, cam);
    this->renderMeshes(shader, cam);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return sceneFrame;
}

void World::renderRefractions(ShaderProgram *shader, ViewCamera *cam){

    glm::mat4 projView = this->getPerspectiveMatrix() * cam->getViewMatrix();

    for(BasicMesh* m: this->meshes)
        if( m->isRefractive() )
            m->renderRefractive(shader, projView);

}

void World::renderMeshes(ShaderProgram *shader, ViewCamera* cam){

    glm::mat4 projView = this->getPerspectiveMatrix() * cam->getViewMatrix();

    for(BasicMesh* m: this->meshes)
        if( !m->isRefractive() )
            m->render(shader, projView);
}

void World::renderSkybox(glm::mat4 viewMatrix){

    if( this->showSkybox && this->skybox != nullptr )
        skybox->render(viewMatrix);

}

void World::renderLights(ShaderProgram *shader){

    // for(Light* l:lights)
    //     l->render(shader);

}

void World::renderShadowCubeMaps(ShaderProgram *shader){

    for( Light* l:lights )
        l->renderShadowCubeMap(shader, this->meshes);

}

void World::renderFloor(ShaderProgram *shader, ViewCamera *cam){

    if(this->floor != nullptr)
        if( !this->floor->isReflective() )
            this->floor->render(shader, this->getPerspectiveMatrix() * cam->getViewMatrix());
        else
            this->floor->mirror(shader, this->meshes, cam, this->getPerspectiveMatrix(), this->skybox);
}

void World::renderReflections(ShaderProgram *shader, ViewCamera *cam){
    if(this->floor != nullptr)
        this->floor->mirror(shader, this->meshes, cam, this->getPerspectiveMatrix(), this->skybox);
}

void World::refreshRefractiveSurroundings(ShaderProgram *shader){

    for(BasicMesh* m: this->meshes)
        if( m->isRefractive() )
            m->renderSurroundings(this->meshes, this->skybox, shader);

}

bool *World::getShowSkyboxReference(){
    return &this->showSkybox;
}
