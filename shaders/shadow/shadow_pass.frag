#version 460 core

in vec3 position;

uniform vec3 lightPosition;

out float lightToPixelDistance;

void main(){
    lightToPixelDistance = length( position - lightPosition ) ;
}