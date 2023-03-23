#include "ViewCamera.h"

ViewCamera::ViewCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up){

    this->position = pos;
    this->front = front;
    this->up = up;

    calculateViewMatrix();

}

glm::mat4 ViewCamera::calculateViewMatrix(){
    
    this->right = glm::normalize( glm::cross( up, front ) );
    this->viewMatrix = glm::lookAt( position, position+front, up );

    this->matrixUpdated = true;

    return this->viewMatrix;

}

glm::mat4 ViewCamera::getViewMatrix(){

    if(!matrixUpdated)
        calculateViewMatrix();

    return this->viewMatrix;
}

void ViewCamera::move(glm::vec3 v){

    this->position += speed*v;
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

    this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->up));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    matrixUpdated = false;

}
