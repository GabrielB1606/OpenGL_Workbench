#ifndef WORLD_H
#define WORLD_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Light.h"
#include "BasicMesh.h"


class World{
private:
    
    std::vector<BasicMesh*> meshes;
    glm::mat4 perspectiveMatrix;

    std::vector<Light*> lights;

    // Define the projection matrix
    float fov = 90.0f;  // Field of view in degrees
    float width = 1280, height = 720;
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 1000.0f; // Far clipping plane

    bool perspectiveUpdated = true;

public:
    World(float fov, float width, float height, float nearClippingPlane, float farClippingPlane);
    virtual ~World();

    void setFOV(float fov);
    void setAspectRatio(float width, float height);

    glm::mat4 getPerspectiveMatrix();
    std::vector<BasicMesh*> getMeshes();
    std::vector<Light*> getLights();
    
    void loadMesh(std::string path);
    void addLight(Light* l);

    void render(ShaderProgram *shader);
};

#endif