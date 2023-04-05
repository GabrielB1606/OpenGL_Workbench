#version 460 core

in vec3 position;

uniform vec3 lightPosition;

out float lightToPixelDistance;

void main(){
    vec3 lightToFragment = position - lightPosition;
    lightToPixelDistance = length( lightToFragment ) ;
}