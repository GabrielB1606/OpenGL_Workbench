#version 460 core

struct Colors{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float useDiffTexture;
	float useSpecTexture;
};

struct PointLight{
	vec3 position;
	vec3 color;
	vec3 intensity;
};

in vec2 texCoord;
in vec3 fragPosition;
in vec3 normal;

uniform PointLight lights[3];
uniform Colors mat;
uniform vec3 cameraPosition;
layout(binding = 0) uniform sampler2D DiffTexture;
layout(binding = 1) uniform sampler2D SpecularTexture;

uniform float useDiffTexture;
uniform float useSpecTexture;

out vec4 FragColor;

vec3 calculateAmbient(PointLight light){
    return mat.ambient * light.color * light.intensity;
}

vec3 calculateDiffuse(PointLight light){
    
    vec3 lightDir = normalize( light.position - fragPosition );
    float factor = max( dot(normalize(normal), lightDir), 0.0 );

    return factor * mat.diffuse;
}

vec3 calculateSpecular(PointLight light, vec3 camPosition){
    
    vec3 viewDirection = normalize( fragPosition - camPosition );

    vec3 lightDirection = normalize( fragPosition - light.position );
    vec3 reflectDirection = normalize( reflect( -lightDirection, normalize(normal) ) );

	float dotProduct = dot( viewDirection, reflectDirection );
	float specularConstant = 0.0;
	if( dotProduct > 0 && dot( viewDirection, lightDirection ) > 0 )
		specularConstant = pow( dotProduct, mat.shininess );

	if( useSpecTexture > 0 )
    	return specularConstant * light.color * texture2D(SpecularTexture, texCoord).xyz;
	else
    	return mat.specular * specularConstant * light.color;
}

void main() {

	vec3 ambientComponent = calculateAmbient(lights[0]);
	vec3 diffuseComponent = calculateDiffuse(lights[0]);

	vec3 specularComponent = vec3(0.0);
	if( mat.shininess >= 0.0 )
		specularComponent = calculateSpecular(lights[0], cameraPosition);

	if ( useDiffTexture > 0 )
		FragColor = texture2D(DiffTexture, texCoord) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
	else
		FragColor = vec4(mat.diffuse, 1.0) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}