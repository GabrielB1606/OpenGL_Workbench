#ifndef VIEWCAMERA_H
#define VIEWCAMERA_H

#include "headers.h"

class ViewCamera{
private:

    float speed = 1.f;
    float sensitivity = 250.f;

    glm::vec3 position;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float pitch = 0.f;
    float yaw = 0.f;
    float roll = 0.f;

    bool matrixUpdated = false;
    glm::mat4 viewMatrix;

public:
    ViewCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
    virtual ~ViewCamera(){}

    glm::mat4 calculateViewMatrix();
    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();

    void move(glm::vec3 v);
    void rotate(glm::vec3 r);
};


#endif