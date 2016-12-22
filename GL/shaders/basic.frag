#version 420 core

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

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

#define POINT_LIGHT_LIMIT 8
uniform PointLight pointLights[POINT_LIGHT_LIMIT];
uniform int activePointLights;

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

void main() {
	
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 result = vec3(0, 0, 0);

	for(int i = 0; i < 2; i++) {
		result += addPointLight(pointLights[i], norm, fragPos, viewDir);
	}

	color = vec4(result, 1.0);
}