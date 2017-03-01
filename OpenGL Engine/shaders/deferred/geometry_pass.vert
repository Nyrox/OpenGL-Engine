#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 tangent;


out VS_OUT {
	vec3 world_fragPos;
	vec3 world_surfaceNormal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#include("headers/common.glsl");

void main() {
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);
	
	vs_out.world_fragPos = vec3(model * vec4(position, 1.0));
	vs_out.world_surfaceNormal = mat3(transpose(inverse(model))) * normal;
}