#ifndef SCENEFBO_H
#define SCENEFBO_H

#include "headers.h"

#include "ShaderProgram.h"
#include "BasicMesh.h"

class SceneFBO{
private:

    size_t width, height;
    GLuint fbo;
    GLuint texMap;

public:
    SceneFBO();
    virtual ~SceneFBO();

    bool init(size_t width, size_t height);
    void bindWrite();
    void bindRead(GLenum textureUnit);
};

#endif