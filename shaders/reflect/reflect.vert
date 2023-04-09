#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 ProjViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;

void main() {
    vec4 pos = ProjViewMatrix * ModelMatrix * vec4(aPos, 1.0);
    gl_Position = pos;

	texCoord = vec2((aPos.x+50)/100, (aPos.z+50)/100);
}