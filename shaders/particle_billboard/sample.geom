#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float u_Size;

out vec2 texCoord;

void main()
{
    // Retrieve the position of the input vertex
    vec4 position = gl_in[0].gl_Position;

    // Calculate the position of the four vertices of the billboard quad
    vec4 v0 = position + u_Size*vec4(-1, -1, 0, 0);
    vec4 v1 = position + u_Size*vec4( 1, -1, 0, 0);
    vec4 v2 = position + u_Size*vec4(-1,  1, 0, 0);
    vec4 v3 = position + u_Size*vec4( 1,  1, 0, 0);

    // Calculate texture coordinates for each vertex
    gl_Position = v0;
    texCoord = vec2(0, 0);
    EmitVertex();
    gl_Position = v1;
    texCoord = vec2(1, 0);
    EmitVertex();
    gl_Position = v2;
    texCoord = vec2(0, 1);
    EmitVertex();
    gl_Position = v3;
    texCoord = vec2(1, 1);
    EmitVertex();

    // Complete the primitive
    EndPrimitive();
}