#version 420 core

in vec3 normal;
in vec3 fragPos;
in vec2 uv;
in vec4 fragPosLightSpace;

out vec4 color;

uniform vec3 cameraPos;



struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient; 
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D shadowMap;

#define POINT_LIGHT_LIMIT 8
uniform PointLight pointLights[POINT_LIGHT_LIMIT];
uniform int activePointLights;

#define DIR_LIGHT_LIMIT 2
uniform DirectionalLight directionalLights[DIR_LIGHT_LIMIT];
uniform int activeDirectionalLights;

uniform Material material;

vec3 addPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	
	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient	= attenuation * light.ambient * vec3(texture(material.diffuse, uv));
	vec3 diffuse	= attenuation * light.diffuse * diff * vec3(texture(material.diffuse, uv));
	vec3 specular	= attenuation * spec * vec3(texture(material.specular, uv));

	return (ambient + diffuse + specular);
}

float shadowLookup(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;


	float bias = 0.002;

	// Do some multisampling
	float shadow = 0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= -1; y++) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	return shadow;
}

vec3 addDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, uv));
	vec3 specular = spec * vec3(texture(material.specular, uv));
	
	
	float shadow = shadowLookup(fragPosLightSpace);

	vec3 result = (ambient + (1.f - shadow) *  (diffuse + specular));
	return result;
}

void main() {
	
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 result = vec3(0, 0, 0);

	result += addDirectionalLight(directionalLights[0], norm, fragPos, viewDir);

	for(int i = 0; i < 0; i++) {
		result += addPointLight(pointLights[i], norm, fragPos, viewDir);
	}

	color = vec4(result, 1.0);
}