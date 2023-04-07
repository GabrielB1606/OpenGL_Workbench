#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color){

    // lightModel = new BasicMesh();
    // lightModel->loadMesh("models/estrellica.obj");
    
    this->position = position;
    this->color = color;

    this->attenuation = glm::vec3(0.f, 0.f, 1.f);

    shadowMap.init(SHADOW_MAP_SIZE);


}

Light::~Light(){
    if( lightModel != nullptr )
        delete lightModel;
}

void Light::sendUniforms(ShaderProgram *shader, size_t index){

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
    shader->setMat4fv( glm::perspective(glm::radians(90.f), 1.f, 0.1f, 1000.f), "ProjectionMatrix", GL_FALSE );
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

    glm::mat4 lightView;

    for (size_t i = 0; i < 6; i++){
        
        shadowMap.bindWrite( GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum)i );
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        lightView = glm::lookAt( this->position, this->position + cubeMapTargets[i], cubeMapUps[i] );
        shader->setMat4fv(lightView, "ViewMatrix", GL_FALSE);
        
        for(BasicMesh* mesh : meshes){
        
            // if( glm::length( glm::normalize(*(mesh->getTranslationReference())) - glm::normalize(this->position + cubeMapTargets[i]) ) <= 1.45f )
                mesh->render(shader);
        
        }

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);    

}

void Light::move(glm::vec3 v){

    this->position += v;

    if( lightModel != nullptr )
        lightModel->translate(v);

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

void Light::attatchModel(BasicMesh *model){

    this->lightModel = model;

}

void Light::loadMesh(std::string filePath){

    this->lightModel = new BasicMesh();
    this->lightModel->loadMesh(filePath);
    this->lightModel->scaleUp(-glm::vec3(0.975f, 0.975f, 0.975f));
	this->lightModel->rotate(glm::vec3(90.f, 0.f, 0.f));
    
}

void Light::render(ShaderProgram *shader){

    if(this->lightModel != nullptr)
        this->lightModel->render(shader);
}
