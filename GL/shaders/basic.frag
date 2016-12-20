#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec3 cameraPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 
uniform Material material;

void main() {
	
	// Ambient
	vec3 ambient = material.ambient * light.ambient;

	// Normals
	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(light.position - fragPos);
	
	// Diffuse
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightdir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}