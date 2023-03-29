#ifndef LIGHT_H
#define LIGHT_H

#include <glm-0.9.9.9/vec3.hpp>
#include "ShaderProgram.h"

class Light{
private:

    glm::vec3 color;
    glm::vec3 position;

public:
    Light(glm::vec3 position, glm::vec3 color = glm::vec3(1.f));
    virtual ~Light(){}

    void sendUniforms(ShaderProgram *shader, int index);

    void move(glm::vec3 v);

};


#endif