#ifndef BASICMESHENTRY_H
#define BASICMESHENTRY_H

#include "headers.h"

struct BasicMeshEntry {
    BasicMeshEntry()
    {
        numIndices = 0;
        baseVertex = 0;
        baseIndex = 0;
        materialIndex = -1;
        name = "";
    }

    std::string name;

    GLuint numIndices;
    GLuint baseVertex;
    GLuint baseIndex;
    GLuint materialIndex;
};

#endif