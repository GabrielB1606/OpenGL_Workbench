#ifndef SHADOWCUBEMAPFBO_H
#define SHADOWCUBEMAPFBO_H

#include "headers.h"

class ShadowCubeMapFBO{
private:

    size_t size;
    GLuint fbo;
    GLuint shadowMap;
    GLuint depth;

public:
    ShadowCubeMapFBO();
    virtual ~ShadowCubeMapFBO();

    bool init(size_t size);
    void bindWrite(GLenum cubeFace);
    void bindRead(GLenum textureUnit);
};

#endif