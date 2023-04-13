#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 ProjViewModelMatrix;

void main() {
    
    vec4 pos = ProjViewModelMatrix * vec4(aPos, 1.0);
    gl_Position = pos;

}