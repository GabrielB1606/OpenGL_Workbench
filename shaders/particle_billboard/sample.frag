#version 460 core

in vec2 texCoord;
layout(binding = 0) uniform sampler2D texture;

uniform vec4 u_Color;

out vec4 fragColor;

void main(){
    fragColor = u_Color * texture2D(texture, texCoord); 
}