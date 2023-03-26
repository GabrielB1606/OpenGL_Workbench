#version 330 core

struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoord;
in vec3 position;
in vec3 normal;

uniform Colors mat;
uniform sampler2D DiffTexture;

out vec4 FragColor;

vec3 calculateAmbient(vec3 ambient, vec3 lightColor, float intensity){
    return ambient * lightColor * intensity;
}

vec3 calculateDiffuse(vec3 diffuse, vec3 position, vec3 normal, vec3 lightPosition){
    
    vec3 lightDir = normalize( lightPosition - position );
    float factor = max( dot(lightDir, normalize(normal)), 0.0 );

    return factor * diffuse;
}

void main() {

	vec4 ambientComponent = vec4(calculateAmbient( mat.ambient, vec3(1.0), 0.2 ), 1.0);
	vec4 diffuseComponent = vec4( calculateDiffuse(mat.diffuse, position, normal, vec3(0.0) ) , 1.0);

	FragColor = texture2D(DiffTexture, texCoord) * (ambientComponent + diffuseComponent);
}