#version 420 core

#include("headers/lighting.glsl");

layout (location = 0) out vec4 albedoSpec;

in VS_OUT {
	vec2 uv;
} fs_in;



struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
	float ambientIntensity;

	float constant;
	float linear;
	float quadratic;
};

uniform int point_light_count;
uniform PointLight point_lights[16];
uniform samplerCube shadow_map_0;
uniform samplerCube shadow_map_1;
uniform samplerCube shadow_map_2;
uniform samplerCube shadow_map_3;

uniform vec3 camera_position;
uniform float shadow_far_plane;

uniform sampler2D tex2D_geoPositions;
uniform sampler2D tex2D_geoNormals;

float point_shadow(PointLight light, samplerCube shadowMap, vec3 fragPos) {
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

void main() {
	vec4 result = vec4(0);

	for(int i = 0; i < point_light_count; i++) {
		vec3 fragmentNormal = texture(tex2D_geoNormals, fs_in.uv).rgb;
		vec3 fragmentPosition = texture(tex2D_geoPositions, fs_in.uv).rgb;

		vec3 lightDir = normalize(point_lights[i].position - fragmentPosition);
		vec3 viewDir = normalize(camera_position - fragmentPosition);

		float shadow = 0;
		if(i == 0) shadow = point_shadow(point_lights[i], shadow_map_0, fragmentPosition);
		if(i == 1) shadow = point_shadow(point_lights[i], shadow_map_1, fragmentPosition);

		float diffuseIntensity = max(dot(fragmentNormal, lightDir), 0.0);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float specularIntensity = pow(max(dot(fragmentNormal, halfwayDir), 0.0), 128);

		float distance = length(point_lights[i].position - fragmentPosition);
		float attenuation = calculateAttenuation(distance, point_lights[i].constant, point_lights[i].linear, point_lights[i].quadratic);

		vec3 diffuse = attenuation * point_lights[i].color * point_lights[i].intensity * diffuseIntensity;
		vec3 ambient = attenuation * point_lights[i].color * (point_lights[i].intensity * point_lights[i].ambientIntensity);

		result.rgb += ambient + (1.0 - shadow) * diffuse;
		result.a += specularIntensity * attenuation;
	}

	albedoSpec = result;
}