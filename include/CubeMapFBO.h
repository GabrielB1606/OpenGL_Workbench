#ifndef CUBEMAPFBO_H
#define CUBEMAPFBO_H

#include "headers.h"

class CubeMapFBO{
private:

    size_t size;
    GLuint fbo;
    GLuint tex;

public:
    CubeMapFBO();
    virtual ~CubeMapFBO();

    bool init(size_t size);
    void bindWrite(GLenum cubeFace);
    void bindRead(GLenum textureUnit);
};

#endif