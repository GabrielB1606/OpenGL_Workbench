#ifndef WORLD_H
#define WORLD_H

#include "headers.h"

#include "ShaderProgram.h"
#include "Light.h"
#include "BasicMesh.h"
#include "Plane.h"
#include "Skybox.h"
#include "SceneFBO.h"

class World{
private:
    
    glm::mat4 perspectiveMatrix;

    std::vector<BasicMesh*> meshes;
    std::vector<Light*> lights;
    std::vector<Plane*> planes;

    Skybox* skybox = nullptr;
    Plane* floor = nullptr;    

    // Define the projection matrix
    float fov = 90.0f;  // Field of view in degrees
    float width = 1280, height = 720;
    float nearPlane = 0.1f;  // Near clipping plane
    float farPlane = 1000.0f; // Far clipping plane

    bool perspectiveUpdated = true;
    bool showSkybox = true;

public:
    World(float fov, float width, float height, float nearClippingPlane, float farClippingPlane);
    virtual ~World();

    float getWidth();
    float getHeight();

    void setFOV(float fov);
    void setAspectRatio(float width, float height);
    
    void createSkybox(ShaderProgram* skyboxProgram, std::string directory, std::string format = "jpg");
    void changeSkybox(std::string directory, std::string format = "jpg");

    void createFloor();

    glm::mat4 getPerspectiveMatrix();
    std::vector<BasicMesh*> getMeshes();
    std::vector<BasicMesh*>* getMeshesVectorReference();
    std::vector<Light*> getLights();
    Plane* getFloor();

    Plane* getPlane(size_t index);

    BasicMesh* getMesh(size_t index);
    Light* getLight(size_t index);
    size_t getMeshesSize();
    
    void loadMesh(std::string path);
    void addLight(Light* l);
    void removeMesh(size_t index);
    void removeLight(size_t index);

    void sendUniforms(ShaderProgram *shader);
    std::shared_ptr<SceneFBO> renderSceneFBO(ShaderProgram *shader, ViewCamera *cam);
    void renderRefractions(ShaderProgram *shader, ViewCamera *cam);
    void renderMeshes(ShaderProgram *shader, ViewCamera *cam);
    void renderPlanes(ShaderProgram *shader, ViewCamera *cam);
    void renderSkybox(glm::mat4 viewMatrix);
    void renderLights(ShaderProgram *shader);
    void renderShadowCubeMaps(ShaderProgram *shader);
    void renderFloor(ShaderProgram *shader, ViewCamera *cam);
    void renderReflections(ShaderProgram *shader, ViewCamera *cam);
    void refreshRefractiveSurroundings(ShaderProgram *shader);

    bool* getShowSkyboxReference();
};

#endif