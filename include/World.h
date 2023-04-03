#ifndef WORLD_H
#define WORLD_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Light.h"
#include "BasicMesh.h"
#include "Skybox.h"

class World{
private:
    
    glm::mat4 perspectiveMatrix;

    std::vector<BasicMesh*> meshes;
    std::vector<Light*> lights;

    Skybox* skybox = nullptr;

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
    void createSkybox(ShaderProgram* skyboxProgram, std::string directory, std::string format = "jpg");
    void changeSkybox(std::string directory, std::string format = "jpg");

    glm::mat4 getPerspectiveMatrix();
    std::vector<BasicMesh*> getMeshes();
    std::vector<BasicMesh*>* getMeshesVectorReference();
    std::vector<Light*> getLights();

    BasicMesh* getMesh(size_t index);
    Light* getLight(size_t index);
    size_t getMeshesSize();
    
    void loadMesh(std::string path);
    void addLight(Light* l);

    void sendUniforms(ShaderProgram *shader);
    void renderMeshes(ShaderProgram *shader);
    void renderSkybox(glm::mat4 viewMatrix);
};

#endif