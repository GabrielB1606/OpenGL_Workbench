#ifndef PLANE_H
#define PLANE_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Material.h"
#include "SceneFBO.h"
#include "BasicMesh.h"
#include "ViewCamera.h"

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
    SceneFBO reflection;

    glm::vec3 position = glm::vec3(0.f);

public:
    Plane(int div, float width, glm::vec3 init_pos = glm::vec3(0.f));
    virtual ~Plane();

    void render(ShaderProgram* shader);
    void mirror(ShaderProgram* shader, std::vector<BasicMesh*> meshes, ViewCamera cam);
};

#endif