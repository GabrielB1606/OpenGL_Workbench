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
	float intensity;

	float quadratic;
	float linear;
	float constant;
};

in vec2 texCoord;
in vec3 fragPosition;
in vec3 normal;

uniform PointLight lights[3];
uniform int nLights;

uniform Colors mat;
uniform vec3 cameraPosition;
layout(binding = 0) uniform sampler2D   DiffTexture;
layout(binding = 1) uniform sampler2D   SpecularTexture;
layout(binding = 2) uniform samplerCube CubeMapLight0;
layout(binding = 3) uniform samplerCube CubeMapLight1;
layout(binding = 4) uniform samplerCube CubeMapLight2;

uniform int useDiffTexture;
uniform int useSpecTexture;

out vec4 FragColor;

float calculateShadow(vec3 lightToPixel, samplerCube shadowMap){

    float dist = length(lightToPixel);
    lightToPixel.y = -lightToPixel.y;
    float sample = texture(shadowMap, lightToPixel).r;
    float bias = 0.015;

    if( smaple + bias < dist )
        return 0.15;
    else
        return 1.0;

}

vec3 calculateAmbient(PointLight light){
    return mat.ambient * light.color * light.intensity;
}

vec3 calculateDiffuse(PointLight light){
    
    vec3 lightDir = normalize( light.position - fragPosition );
    float factor = max( dot(normalize(normal), lightDir), 0.0 );

    return factor * mat.diffuse  * light.color;
}

vec3 calculateSpecular(PointLight light, vec3 camPosition){
    
    vec3 viewDirection = normalize( fragPosition - camPosition );

    vec3 lightDirection = normalize( fragPosition - light.position );
    vec3 reflectDirection = normalize( reflect( -lightDirection, normalize(normal) ) );

	float dotProduct = dot( viewDirection, reflectDirection );
	float specularConstant = 0.0;
	if( dotProduct > 0 && dot( viewDirection, lightDirection ) > 0 )
		specularConstant = pow( dotProduct, mat.shininess );

	if( useSpecTexture == 1 )
    	return specularConstant * light.color * texture2D(SpecularTexture, texCoord).xyz;
	else
    	return mat.specular * specularConstant * light.color;
}


float calculateAttenuation(PointLight light){

    float dist = length( light.position - fragPosition );
    return 1.f/ ( light.constant + (light.linear*dist) +  ( light.quadratic*dist*dist ) );

}

void main() {
	vec3 ambientComponent = vec3(0.0);
	vec3 diffuseComponent = vec3(0.0);
	vec3 specularComponent = vec3(0.0);
	float attenuation = 1.f;

	for(int i = 0; i<nLights; i++){

		attenuation = calculateAttenuation(lights[i]);

		ambientComponent += calculateAmbient(lights[i]) * attenuation;
		diffuseComponent += calculateDiffuse(lights[i]) * attenuation;
		
		if( mat.shininess >= 0.0 )
			specularComponent += calculateSpecular(lights[i], cameraPosition) * attenuation;
	}


	if ( useDiffTexture == 1 )
		FragColor = texture2D(DiffTexture, texCoord) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
	else
		FragColor = vec4(mat.diffuse, 1.0) * vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
}