#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color){

    this->position = position;
    this->color = color;

    this->attenuation = glm::vec3(0.f, 0.f, 1.f);

    shadowMap.init(1024);

}

void Light::sendUniforms(ShaderProgram *shader, int index){

    shader->setVec3f(this->position, ("lights[" + std::to_string(index) + "].position").c_str() );
    shader->setVec3f(this->color, ("lights[" + std::to_string(index) + "].color").c_str());
    shader->set1f(this->intensity, ("lights[" + std::to_string(index) + "].intensity").c_str());
    
    shader->set1f(this->attenuation.x , ("lights[" + std::to_string(index) + "].quadratic").c_str());
    shader->set1f(this->attenuation.y, ("lights[" + std::to_string(index) + "].linear").c_str());
    shader->set1f(this->attenuation.z, ("lights[" + std::to_string(index) + "].constant").c_str());

    shader->use();
    
    switch (index)
    {
    case 0:
        shadowMap.bindRead(GL_TEXTURE2);
        break;
    case 1:
        shadowMap.bindRead(GL_TEXTURE3);
        break;
    case 2:
        shadowMap.bindRead(GL_TEXTURE4);
        break;
    
    default:
        break;
    }
    
    shader->stopUsing();
    
}

glm::vec3 cubeMapTargets[6] = {
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, -1.0f)
};

glm::vec3 cubeMapUps[6] = {
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
};

void Light::renderShadowCubeMap( ShaderProgram *shader, std::vector<BasicMesh*> meshes){

    shader->setVec3f(this->position, "lightPosition");
    shader->setMat4fv( glm::perspective(90.f, 1.f, 0.1f, 1000.f), "ProjectionMatrix", GL_FALSE );
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    glm::mat4 lightView;

    for (size_t i = 0; i < 6; i++){
        
        shadowMap.bindWrite( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        lightView = glm::lookAt( this->position, cubeMapTargets[i], cubeMapUps[i] );
        shader->setMat4fv(lightView, "ViewMatrix", GL_FALSE);
        
        for(BasicMesh* mesh : meshes)
            mesh->render(shader);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);    

}

void Light::move(glm::vec3 v){

    this->position += v;

}

glm::vec3 *Light::getPositionReference(){
    return &this->position;
}

glm::vec3 *Light::getAttentionReference(){
    return &this->attenuation;
}

glm::vec3 *Light::getColorReference(){
    return &this->color;
}

float *Light::getIntensityReference(){
    return &this->intensity;
}
