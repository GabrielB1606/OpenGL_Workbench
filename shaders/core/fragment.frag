#version 330 core

struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoord;

uniform Colors mat;
uniform sampler2D DiffTexture;

out vec4 FragColor;

vec3 calculateAmbient(vec3 ambient, vec3 lightColor, float intensity){
    return ambient * lightColor * intensity;
}

void main() {

	vec4 ambientComponent = vec4(calculateAmbient( mat.ambient, vec3(1.0), 0.5 ), 1.0);

	FragColor = texture2D(DiffTexture, texCoord) * ambientComponent;
}