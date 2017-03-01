#version 420 core

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;

in VS_OUT {
	vec3 world_fragPos;
	vec3 world_surfaceNormal;
} fs_in;

void main() {
	position = fs_in.world_fragPos;
	normal = fs_in.world_surfaceNormal;
}