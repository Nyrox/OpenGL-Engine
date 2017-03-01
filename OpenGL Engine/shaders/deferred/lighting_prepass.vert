#version 420 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

out VS_OUT {
	vec2 uv;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#include("headers/common.glsl");

void main() {
	gl_Position = vec4(position.x - 1, position.y - 1, 0.0f, 1.0f);
	vs_out.uv = uv;
}