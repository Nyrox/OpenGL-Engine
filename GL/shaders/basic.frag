#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
	float ambientStrength = 0.1f; 
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightdir = normalize(lightPos - fragPos);

	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;

	color = vec4(result, 1.0f);
}