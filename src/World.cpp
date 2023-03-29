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

void World::loadMesh(std::string path){

    meshes.push_back( new BasicMesh() );
    meshes[ meshes.size()-1 ]->loadMesh(path);

}

void World::render(ShaderProgram *shader){

    for(BasicMesh* m: this->meshes)
        m->render(shader);

}
