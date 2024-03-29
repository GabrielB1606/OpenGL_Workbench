#include "ViewCamera.h"

ViewCamera::ViewCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up){

    this->position = pos;
    this->front = front;
    this->up = up;

    this->pitch = 0.f;
    this->yaw = 90.f;
    this->roll = 0.f;

    calculateViewMatrix();

}

glm::mat4 ViewCamera::calculateViewMatrix(){
    
    this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->front));
    this->up = glm::normalize(glm::cross(this->front, this->right));

    this->viewMatrix = glm::lookAt( position, position+front, up );

    this->matrixUpdated = true;

    return this->viewMatrix;

}

glm::mat4 ViewCamera::getViewMatrix(){

    if(!matrixUpdated)
        calculateViewMatrix();

    return this->viewMatrix;
}

glm::vec3 ViewCamera::getPosition(){
    return this->position;
}

glm::vec3 ViewCamera::getTarget(){
    if(!matrixUpdated)
        calculateViewMatrix();

    return position+front;
}

glm::vec3 ViewCamera::getUp(){
    if(!matrixUpdated)
        calculateViewMatrix();
        
    return up;
}

void ViewCamera::sendUniforms(ShaderProgram *shader){
    shader->setMat4fv(this->getViewMatrix(), "ViewMatrix", GL_FALSE);
    shader->setVec3f(this->getPosition(), "cameraPosition" );
}

void ViewCamera::move(glm::vec3 v){

    this->position += speed*v.z*this->front;
    this->position += speed*v.x*this->right;
    this->position += speed*v.y*this->up;

    this->matrixUpdated = false;

}

void ViewCamera::walk(glm::vec3 v){

    this->position.y = 0.f;

    this->position += speed*(v.z + v.y)* glm::vec3( front.x, 0.f, front.z + abs(front.y) );
    this->position += speed*v.x* glm::vec3( right.x + right.y, 0.f, right.z );

    this->matrixUpdated = false;

}

void ViewCamera::rotate(glm::vec3 r){

    this->pitch += r.y * this->sensitivity;
    this->yaw += r.x * this->sensitivity;

    if( this->pitch > 80.f )
        this->pitch = 80.f;
    else if(this->pitch < -80.f)
        this->pitch = -80.f;

    if( this->yaw > 360.f || this->yaw < -360.f )
        this->yaw = 0.f;

    matrixUpdated = false;

}

float *ViewCamera::getSensitivyReference(){
    return &this->sensitivity;
}

float *ViewCamera::getSpeedReference(){
    return &this->speed;
}
