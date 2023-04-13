#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 ProjViewModelMatrix;
uniform mat4 ModelMatrix;

out vec3 position;

void main(){
    gl_Position = ProjViewModelMatrix * vec4(aPos, 1.0);
    position = ( ModelMatrix * vec4(aPos, 1.0) ).xyz;
}