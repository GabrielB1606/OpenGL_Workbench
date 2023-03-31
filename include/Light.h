#ifndef LIGHT_H
#define LIGHT_H

#include "headers.h"

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