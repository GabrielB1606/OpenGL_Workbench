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
    return this->viewMatrix;

}

glm::mat4 ViewCamera::getViewMatrix(){
    return this->viewMatrix;
}
