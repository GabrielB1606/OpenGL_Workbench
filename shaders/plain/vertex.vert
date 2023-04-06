#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 ProjViewMatrix;
uniform mat4 ModelMatrix;

void main() {
    gl_Position = ProjViewMatrix * ModelMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
}