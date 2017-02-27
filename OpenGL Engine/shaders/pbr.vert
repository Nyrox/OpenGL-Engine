#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

out VS_OUT {
	vec3 faceNormal;
	vec3 fragPos;
	vec2 uv;

	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#include("headers/common.glsl");

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	
	vs_out.faceNormal = mat3(transpose(inverse(model))) * normal;
	vs_out.fragPos = vec3(model * vec4(position, 1.0));
	vs_out.uv = uv;
	vs_out.TBN = createTBNMatrix(model, normal, tangent);
}