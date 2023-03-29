#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color){

    this->position = position;
    this->color = color;

}

void Light::sendUniforms(ShaderProgram *shader, int index){

    shader->setVec3f(this->position, ("lights[" + std::to_string(index) + "].position").c_str() );
    shader->setVec3f(this->color, ("lights[" + std::to_string(index) + "].color").c_str());

}

void Light::move(glm::vec3 v){

    this->position += v;

}
