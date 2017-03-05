#version 420 core

#include("headers/lighting.glsl");

in VS_OUT {
	vec2 uv;
} fs_in;

layout (location = 0) out vec3 outRadiance;
layout (location = 1) out vec3 outBrdf;
layout (location = 2) out vec3 outKd;

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 camera_position;
uniform float shadow_far_plane;

uniform int point_light_count;
uniform PointLight point_lights[16];
uniform samplerCube shadow_map_0;
uniform samplerCube shadow_map_1;
uniform samplerCube shadow_map_2;
uniform samplerCube shadow_map_3;

uniform sampler2D tex2D_geoPositions;
uniform sampler2D tex2D_geoNormals;
uniform sampler2D tex2D_geoRoughnessMetal;
uniform sampler2D tex2D_geoAlbedo;


/*
Legend:

fragPos: Fragment position in world space
N: World space fragment normal
V: View direction vector
*/

float getPointShadow(samplerCube shadowMap, vec3 fragPos, vec3 lightPos, float shadow_far_plane) {
	vec3 fragToLight = fragPos - lightPos;

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
	vec3 fragPos = normalize(texture(tex2D_geoPositions, fs_in.uv).rgb);
	vec3 N = normalize(texture(tex2D_geoNormals, fs_in.uv).rgb);
	vec3 V = normalize(camera_position - fragPos);

	float roughness = texture(tex2D_geoRoughnessMetal, fs_in.uv).r;
	float metallic = texture(tex2D_geoRoughnessMetal, fs_in.uv).g;
	vec3 albedo = texture(tex2D_geoAlbedo, fs_in.uv).rgb;

	// Base IOR for Dialectrics
	vec3 F0 = vec3(0.04);
	F0      = mix(F0, albedo, metallic);
	
	vec3 radTotal = vec3(0);
	vec3 kdTotal = vec3(0);
	vec3 brdfTotal = vec3(0);

	for(int i = 0; i < point_light_count; i++) {
		vec3 L = normalize(point_lights[i].position - fragPos);
		vec3 H = normalize(V + L);

		float distance = length(point_lights[i].position - fragPos);
		//float attenuation = calculateAttenuation(distance, point_lights[i].constant, point_lights[i].linear, point_lights[i].quadratic);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = point_lights[i].color * point_lights[i].intensity * attenuation;


		// Cook-Torrance
		float NDF = GGX_NormalDistribution(N, H, roughness);
		float G = SmithMicrofacetSelfShadowing(N, V, L, roughness);
		vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);


		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		// Calculate the Cook-Torrance BRDF
		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 brdf = nominator / denominator;


		

		float NdotL = max(dot(N, L), 0.0);
		//Lo += (diffuse * albedo / PI + brdf) * radiance * NdotL;

		float shadow = 0;
		if(i == 0) shadow = getPointShadow(shadow_map_0, fragPos, point_lights[i].position, shadow_far_plane);
		if(i == 1) shadow = getPointShadow(shadow_map_1, fragPos, point_lights[i].position, shadow_far_plane);

		//radTotal += radiance * NdotL;
		//brdfTotal += brdf;
		//kdTotal += (kD * albedo / PI);
		radTotal += (kD * albedo / PI + brdf) * radiance * NdotL; 
	}

	outRadiance = radTotal;
	//outKd = kdTotal;
	outKd = N;
	outBrdf = brdfTotal;
}