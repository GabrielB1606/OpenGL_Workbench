#include "Material.h"

Material::~Material(){
    
    for (size_t i = 0; i < 2; i++)
        if( textures[i] != nullptr )
            delete textures[i];

    
}

void Material::sendUniforms(ShaderProgram *shader){

        // Send Colors
    shader->setVec3f(ambientColor, "mat.ambient");
    shader->setVec3f(diffuseColor, "mat.diffuse");
    shader->setVec3f(specularColor, "mat.specular");

    shader->use();

        // Send Textures
    if( this->textures[ Texture::TYPE::DIFFUSE ] != nullptr )
        this->textures[ Texture::TYPE::DIFFUSE ]->bind(GL_TEXTURE0);
        
    if( this->textures[ Texture::TYPE::SPECULAR ] != nullptr )
        this->textures[ Texture::TYPE::SPECULAR ]->bind(GL_TEXTURE1);

    shader->stopUsing();

}

Material::Material(){
    textures.fill(nullptr);
}