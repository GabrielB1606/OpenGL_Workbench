#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color){

    this->position = position;
    this->color = color;

    this->attenuation = glm::vec3(0.f, 0.f, 1.f);

}

void Light::sendUniforms(ShaderProgram *shader, int index){

    shader->setVec3f(this->position, ("lights[" + std::to_string(index) + "].position").c_str() );
    shader->setVec3f(this->color, ("lights[" + std::to_string(index) + "].color").c_str());
    shader->set1f(this->intensity, ("lights[" + std::to_string(index) + "].intensity").c_str());
    
    shader->set1f(this->attenuation.x , ("lights[" + std::to_string(index) + "].quadratic").c_str());
    shader->set1f(this->attenuation.y, ("lights[" + std::to_string(index) + "].linear").c_str());
    shader->set1f(this->attenuation.z, ("lights[" + std::to_string(index) + "].constant").c_str());

}

void Light::move(glm::vec3 v){

    this->position += v;

}

glm::vec3 *Light::getPositionReference(){
    return &this->position;
}

glm::vec3 *Light::getAttentionReference(){
    return &this->attenuation;
}

float *Light::getIntensityReference(){
    return &this->intensity;
}
