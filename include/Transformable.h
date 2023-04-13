#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "headers.h"

class Transformable{
protected:

    glm::mat4 modelMatrix = glm::mat4(1.f);
    glm::mat4 invModelMatrix = glm::mat4(1.f);

    std::shared_ptr<glm::vec3> translation;
    std::shared_ptr<glm::mat4> rotation;
    std::shared_ptr<glm::vec3> scale;

public:
    Transformable();
    virtual ~Transformable();

    // translation
    void translate(glm::vec3 v);
    void bindTranslation(std::shared_ptr<glm::vec3> v);
    glm::vec3 getTranslation();
    std::shared_ptr<glm::vec3> getTranslationReference();

    // rotation
    void rotate(glm::vec3 v);
    void bindRotation(std::shared_ptr<glm::mat4> m);
    glm::mat4 getRotation();
    std::shared_ptr<glm::mat4> getRotationReference();

    // scale
    void scaleUp(glm::vec3 v);
    void uniformScaleUp(float s);
    void setScale(glm::vec3 v);
    void bindScale(std::shared_ptr<glm::vec3> v);
    glm::vec3 getScale();
    std::shared_ptr<glm::vec3> getScaleReference();

    // Model Matrix
    void calculateModelMatrix();
    glm::mat4 getModelMatrix();
    glm::mat4 getInverseModelMatrix();

};

#endif
