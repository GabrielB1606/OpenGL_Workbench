#version 460 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D SceneTexture;

void main(){
    fragColor = texture(SceneTexture, texCoord); 
}