#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color){

    this->position = position;
    this->color = color;

}

void Light::sendUniforms(ShaderProgram *shader){

    shader->setVec3f(this->position, "lightPosition");
    shader->setVec3f(this->color, "lightColor");

}

void Light::move(glm::vec3 v){

    this->position += v;

}
