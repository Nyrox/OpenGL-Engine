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

struct Light {
    vec3 position;

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

uniform Light light;
uniform Material material;

void main() {

	// Ambient
	vec3 ambient = vec3(texture(material.diffuse, uv)) * light.ambient;

	// Normals
	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(light.position - fragPos);

	// Diffuse
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, uv)));

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightdir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
}