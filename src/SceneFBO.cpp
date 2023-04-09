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
