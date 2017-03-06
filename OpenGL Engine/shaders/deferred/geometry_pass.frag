#version 420 core

#include("headers/lighting.glsl");

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec2 roughnessMetal;
layout (location = 3) out vec3 albedo;

in VS_OUT {
	vec3 world_fragPos;
	vec3 world_surfaceNormal;
	vec2 uv;
	mat3 TBN;
} fs_in;

uniform PbrMaterial material;

void main() {
	position = fs_in.world_fragPos;
	vec3 t_normal = normalize(texture(material.normal, fs_in.uv).rgb);
	normal = normalize(fs_in.TBN * (t_normal));
	normal = fs_in.world_surfaceNormal;

	roughnessMetal.r = texture(material.roughness, fs_in.uv).r;
	roughnessMetal.g = texture(material.metal, fs_in.uv).r;
	albedo = texture(material.albedo, fs_in.uv).rgb;
}