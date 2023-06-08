#include "Texture.h"

void Texture::bind( const unsigned int texUnit ){
    if( this->texture != 0 ){
        glActiveTexture(texUnit);
        glBindTexture( this->target, this->texture );
    }
}

void Texture::unbind(const unsigned int texUnit){
    glActiveTexture(texUnit);
    glBindTexture(this->target, 0);
}

void Texture::setDelTexture(bool val){
    this->delTexture = val;
}

Texture::Texture(std::string filename, unsigned int target){

    this->target = target;

    // load image
    unsigned char *data = stbi_load(filename.c_str() , &this->width, &this->height, &this->nChannels, 0);
    
    glGenTextures(1, &this->texture);
    glBindTexture(this->target, this->texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // generate the texture
    
    if (data){

        if( filename.find(".png") != std::string::npos )
            glTexImage2D(this->target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(this->target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
        glGenerateMipmap(this->target);
    
    }else{
        std::cout << "Failed to load texture " << filename << ": " << stbi_failure_reason() << std::endl;
    }

    // glActiveTexture(0);
    glBindTexture(this->target, 0);
    stbi_image_free(data);

}

Texture::~Texture(){

    if(delTexture)
        glDeleteTextures(1, &this->texture);

}