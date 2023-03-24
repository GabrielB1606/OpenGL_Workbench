#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <array>
#include <glm-0.9.9.9/vec3.hpp>
#include "Texture.h"

struct PBRMaterial{
    float roughness = 0.0f;
    bool isMetal = false;
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
};


class Material {

 public:
    virtual ~Material();

    glm::vec3 ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 specularColor = glm::vec3(0.0f, 0.0f, 0.0f);

    PBRMaterial PBRmaterial;

    // TODO: need to deallocate these
    std::array<Texture *, Texture::TYPE::N_TYPES> textures;
    // Texture* pDiffuse = NULL; // base color of the material
    // Texture* pSpecularExponent = NULL;
};


#endif