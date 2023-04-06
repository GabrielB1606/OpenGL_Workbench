#ifndef PLANE_H
#define PLANE_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Material.h"

class Plane{
private:
    
    int div;
    float width;

    GLuint VAO, VBO, EBO, TBO, NBO;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
    std::vector<glm::vec2> texCoords;

    Material material;

public:
    Plane(int div, float width, glm::vec3 init_pos = glm::vec3(0.f));
    virtual ~Plane();

    void render(ShaderProgram* shader);
};

#endif