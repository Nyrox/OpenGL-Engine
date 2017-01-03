#version 420 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

/*
struct DirectionalLight {
	vec3 direction;

	vec3 ambient; 
	vec3 diffuse;
	vec3 specular;
};*/

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 color;

uniform Material material;


uniform PointLight point_lights[16];
uniform int point_light_count;

uniform samplerCube shadow_map_0;
uniform samplerCube shadow_map_1;

uniform vec3 camera_position;

uniform float shadow_far_plane;



float shadow(PointLight light, samplerCube shadowMap, vec3 fragPos) {
	vec3 fragToLight = fragPos - light.position; 
    float closestDepth = texture(shadowMap, fragToLight).r;
	//color = vec4(vec3(closestDepth), 1.0);

	closestDepth *= shadow_far_plane;
	
	float currentDepth = length(fragToLight);
	
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	//color = vec4(vec3(closestDepth / far_plane), 1.0);

	return shadow;
}

vec3 addPointLight(int index, PointLight light, samplerCube shadowMap, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// If the point light index is not actually used, just add nothing to the output
	if(point_light_count <= index) {
		return vec3(0);	
	}

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
	
	float shadow = shadow(light, shadowMap, fragPos);
	//float shadow = 0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
	 return lighting;
}
/*
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
}*/

void main() {
	
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(camera_position - fragPos);

	vec3 result = vec3(0, 0, 0);

	//result += addDirectionalLight(directionalLights[0], norm, fragPos, viewDir);

	result += addPointLight(0, point_lights[0], shadow_map_0, norm, fragPos, viewDir);
	result += addPointLight(1, point_lights[1], shadow_map_1, norm, fragPos, viewDir);

	color = vec4(result, 1.0);
}