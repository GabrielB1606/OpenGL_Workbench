#include "Material.h"

Material::~Material(){
    {
        for (size_t i = 0; i < 2; i++)
            if( textures[i] != nullptr )
                delete textures[i];

    }
}