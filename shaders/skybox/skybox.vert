#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 texCoord;

void main() {
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(aPos.x , aPos.y, aPos.z, 1.0);
    gl_Position = vec4(pos.x , pos.y, pos.w, pos.w);

	texCoord = vec3(aPos.x, aPos.y, -aPos.z);
}