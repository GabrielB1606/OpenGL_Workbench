#ifndef VIEWCAMERA_H
#define VIEWCAMERA_H

#include "headers.h"

class ViewCamera{
private:

    float speed = 0.5f;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    glm::mat4 viewMatrix;

public:
    ViewCamera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
    virtual ~ViewCamera(){}

    glm::mat4 calculateViewMatrix();
    glm::mat4 getViewMatrix();

    void move(glm::vec3 v, bool recalculate = true);
};


#endif