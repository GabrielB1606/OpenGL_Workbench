#ifndef TEXTURE_H
#define TEXTURE_H

#include "headers.h"

class Texture{
private:
    /* data */
    unsigned int texture;
    unsigned int target;
    int width, height, nChannels;
public:

    enum TYPE{ DIFFUSE = 0, SPECULAR, N_TYPES };

    Texture(std::string filename, unsigned int target);
    ~Texture();

    void bind( const unsigned int texUnit  );
    void unbind(const unsigned int texUnit);

    unsigned int getID() { return texture; }
};

#endif