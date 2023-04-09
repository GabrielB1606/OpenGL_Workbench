#include "SceneFBO.h"

SceneFBO::SceneFBO(){
    this->fbo = 0;
    this->texMap = 0;
}

SceneFBO::~SceneFBO(){

    if( this->fbo != 0 )
        glDeleteFramebuffers(1, &this->fbo);
    
    if( this->texMap != 0 )
        glDeleteTextures(1, &this->texMap);

}

bool SceneFBO::init(size_t width, size_t height){

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texMap);
    glBindTexture(GL_TEXTURE_2D, texMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texMap, 0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);    

    return true;
}

void SceneFBO::bindWrite(){

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);

}

void SceneFBO::bindRead(GLenum textureUnit){

    // Bind the texture
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texMap);

    // Set the texture uniform in the shader
    // GLuint tex_loc = glGetUniformLocation(shader_program, "tex");
    // glUniform1i(tex_loc, 0);

}
