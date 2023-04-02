#version 460 core

in vec3 texCoord;

out vec4 fragColor;

uniform samplerCube SkyboxTexture;

void main(){
    fragColor = texture(SkyboxTexture, texCoord); 
}