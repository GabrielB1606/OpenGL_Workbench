#ifndef LIGHT_H
#define LIGHT_H

#include "headers.h"

#include "ShaderProgram.h"
#include "BasicMesh.h"
#include "ShadowCubeMapFBO.h"

class Light{
private:

    glm::vec3 color;
    std::shared_ptr<glm::vec3> position;
    float intensity = 0.05f;
    glm::vec3 attenuation;
    ShadowCubeMapFBO shadowMap;

public:
    Light(glm::vec3 position = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f));
    Light(std::shared_ptr<glm::vec3> position_ptr, glm::vec3 color = glm::vec3(1.f));
    virtual ~Light();

    void sendUniforms(ShaderProgram *shader, size_t index);
    void renderShadowCubeMap(ShaderProgram *shader, std::vector<BasicMesh*> meshes);

    void move(glm::vec3 v);

    // getters
    std::shared_ptr<glm::vec3> getPositionReference();
    glm::vec3* getAttentionReference();
    glm::vec3* getColorReference();
    float* getIntensityReference();

    // setters
    void attatchPosition(std::shared_ptr<glm::vec3> position);

};


#endif