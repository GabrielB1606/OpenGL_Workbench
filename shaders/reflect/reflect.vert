#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;

void main() {
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
    gl_Position = ModelMatrix * pos;

	texCoord = vec2(pos.x, pos.y);
}