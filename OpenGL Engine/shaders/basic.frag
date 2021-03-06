#version 420 core

/*
	Main fragment shader featuring point and directional lights aswell as shadow mapping.
*/

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


in vec3 fragPos;
in vec2 uv;

out vec4 color;

uniform Material material;


uniform PointLight point_lights[16];
uniform int point_light_count;

uniform DirectionalLight directional_lights[4];
uniform int directional_light_count;

uniform samplerCube shadow_map_0;
uniform samplerCube shadow_map_1;
uniform samplerCube shadow_map_2;
uniform samplerCube shadow_map_3;

uniform sampler2D dir_shadow_map_0;
uniform sampler2D dir_shadow_map_1;

uniform vec3 camera_position;

uniform float shadow_far_plane;


in VS_OUT {
	mat3 TBN;
} fs_in;

#include("headers/lighting.glsl");

float shadow(PointLight light, samplerCube shadowMap, vec3 fragPos) {
	vec3 fragToLight = fragPos - light.position;

	float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.15;
	float samples = 6;

	float viewDistance = length(camera_position - fragPos);
	float diskRadius = (1.0 + (viewDistance / shadow_far_plane)) / 25.0;  

	vec3 sampleOffsetDirections[20] = vec3[] (
		vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
		vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
		vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
		vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
		vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);

	for(int i = 0; i < samples; ++i) {
		float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= shadow_far_plane;
		if(currentDepth - bias > closestDepth) {
			shadow += 1.0;
		}
	}

	shadow /= float(samples);

	return shadow;
}

vec3 addPointLight(int index, PointLight light, samplerCube shadowMap, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// If the point light index is not actually used, just add nothing to the output
	if(point_light_count <= index) {
		return vec3(0, 0, 0);
	}

	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = calculateAttenuation(distance, light.constant, light.linear, light.quadratic);

	vec3 ambient	= attenuation * light.ambient * vec3(texture(material.diffuse, uv));
	vec3 diffuse	= attenuation * light.diffuse * diff * vec3(texture(material.diffuse, uv));
	vec3 specular	= attenuation * spec * vec3(texture(material.specular, uv).r);

	float shadow = shadow(light, shadowMap, fragPos);
	
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
	 return lighting;
}

float shadowLookup(sampler2D shadowMap, vec4 fragPosLightSpace) {
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

vec3 addDirectionalLight(int index, DirectionalLight light, sampler2D shadow_map, vec3 normal, vec4 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, uv));
	vec3 specular = spec * vec3(texture(material.specular, uv).r) * vec3(texture(material.diffuse, uv));


	float shadow = shadowLookup(shadow_map, fragPos);
	
	vec3 result = (ambient + (1.f - shadow) *  (diffuse + specular));
	return result;
}

in vec4 frag_pos_dir_light_space_0;

in vec3 surfaceNormal;
uniform sampler2D normalMap;	
uniform bool useNormalMap;

void main() {

	//vec3 normal = normalize(texture(normalMap, uv).rgb * 2.0 - 1.0);
	//normal = normalize(fs_in.TBN * normal);
	vec3 normal = surfaceNormal;

	vec3 viewDir = normalize(camera_position - fragPos);

	vec3 result = vec3(0, 0, 0);

	//result += addDirectionalLight(0, directional_lights[0], dir_shadow_map_0, normal, frag_pos_dir_light_space_0, viewDir);

	result += addPointLight(0, point_lights[0], shadow_map_0, normal, fragPos, viewDir);
	result += addPointLight(1, point_lights[1], shadow_map_1, normal, fragPos, viewDir);
	//result += addPointLight(2, point_lights[2], shadow_map_2, normal, fragPos, viewDir);

	color = vec4(result, texture(material.diffuse, uv).a);
	//color = vec4(uv.x, uv.y, 0.0, 1.0);

}