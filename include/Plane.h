#ifndef PLANE_H
#define PLANE_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Material.h"

class Plane{
private:
    
    int div;
    float width;

    unsigned int VAO, VBO, EBO;

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    Material material;

public:
    Plane(int div, float width);
    virtual ~Plane(){}

    void render(ShaderProgram* shader);
};

#endif