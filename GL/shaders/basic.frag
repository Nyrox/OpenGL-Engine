#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {
	
	// Ambient
	float ambientStrength = 0.1f; 
	vec3 ambient = ambientStrength * lightColor;

	// Normals
	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(lightPos - fragPos);
	
	// DIffuse
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightdir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	color = vec4(result, 1.0f);
}