#include "Transformable.h"

Transformable::Transformable(){
    translation = std::shared_ptr<glm::vec3>(new glm::vec3(0.f));
    scale = std::shared_ptr<glm::vec3>(new glm::vec3(1.f));
    rotation = std::shared_ptr<glm::mat4>(new glm::mat4(1.f));
}

Transformable::~Transformable(){
    this->translation.reset();
}

void Transformable::translate(glm::vec3 v){
    *this->translation += v;
}

void Transformable::bindTranslation(std::shared_ptr<glm::vec3> v){
    this->translation = v;
}

glm::vec3 Transformable::getTranslation(){
    return *this->translation;
}

std::shared_ptr<glm::vec3> Transformable::getTranslationReference(){
    return this->translation;
}

void Transformable::rotate(glm::vec3 v){

    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(v.x), glm::vec3(1.f, 0.f, 0.f) ) * (*this->rotation);
    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(v.y), glm::vec3(0.f, 1.f, 0.f) ) * (*this->rotation);
    (*this->rotation) = glm::rotate( glm::mat4(1.f), glm::radians(v.z), glm::vec3(0.f, 0.f, 1.f) ) * (*this->rotation);

    this->calculateModelMatrix();

}

void Transformable::bindRotation(std::shared_ptr<glm::mat4> m){
    this->rotation = m;
}

glm::mat4 Transformable::getRotation(){
    return *this->rotation;
}

std::shared_ptr<glm::mat4> Transformable::getRotationReference(){
    return this->rotation;
}

void Transformable::scaleUp(glm::vec3 v){
    *this->scale += v;
}

void Transformable::uniformScaleUp(float s){
    *this->scale += glm::vec3(s);
}

void Transformable::bindScale(std::shared_ptr<glm::vec3> v){
    this->scale = v;
}

glm::vec3 Transformable::getScale(){
    return *this->scale;
}

std::shared_ptr<glm::vec3> Transformable::getScaleReference(){
    return this->scale;
}

void Transformable::calculateModelMatrix(){
    this->modelMatrix = glm::mat4(1.f);
    this->modelMatrix = glm::translate( this->modelMatrix, *this->translation.get() );
    this->modelMatrix = this->modelMatrix * (*this->rotation.get());
    this->modelMatrix = glm::scale(this->modelMatrix, *this->scale.get());
    this->invModelMatrix = glm::inverse(this->modelMatrix);
}

glm::mat4 Transformable::getModelMatrix(){
    return this->modelMatrix;
}

glm::mat4 Transformable::getInverseModelMatrix(){
    return this->invModelMatrix;
}
