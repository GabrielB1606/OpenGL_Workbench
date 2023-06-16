#ifndef PLANE_H
#define PLANE_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Material.h"
#include "SceneFBO.h"
#include "BasicMesh.h"
#include "ViewCamera.h"
#include "Skybox.h"
#include "Transformable.h"

class Plane : public Transformable{
private:
    
    int div;
    float width;

    GLuint VAO, VBO, EBO, TBO, NBO;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
    std::vector<glm::vec2> texCoords;

    Material material;
    glm::mat4 reflection;
    glm::vec4 plane;
    glm::vec3 normal;

    bool reflective = false;
    bool shadowReceiver = true;

public:
    Plane(int div, float width, glm::vec3 init_pos = glm::vec3(0.f));
    virtual ~Plane();

    void render(ShaderProgram* shader);
    void render(ShaderProgram* shader, glm::mat4 projViewMatrix);
    void mirror(ShaderProgram* shader, std::vector<BasicMesh*> meshes, ViewCamera *cam, glm::mat4 projectionMatrix, Skybox* sky = nullptr);
    // void mirror(ShaderProgram* shader, std::vector<BasicMesh*> meshes, glm::mat4 projViewMatrix, glm::mat4 viewMatrix, Skybox* sky = nullptr);

    bool isReflective();
    bool* getReflectiveReference();

    void calculateReflectionMatrix();

};

#endif