#version 420 core
#extension GL_ARB_explicit_uniform_location: enable

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

$(__MATERIAL__);


layout (location = 15, binding = 0) uniform sampler2D tex2D_albedo;
layout (location = 16, binding = 1) uniform sampler2D tex2D_normal;
layout (location = 17, binding = 2) uniform sampler2D tex2D_roughness;
layout (location = 18, binding = 3) uniform sampler2D tex2D_metal;



void main() {
	position = fs_in.world_fragPos;
	//vec3 t_normal = normalize(texture(material.normal, fs_in.uv).rgb);
	//normal = normalize(fs_in.TBN * (t_normal));
	normal = mat_getNormal();

	roughnessMetal.r = mat_getRoughness();
	roughnessMetal.g = mat_getMetallic();
	albedo = mat_getAlbedo();

}