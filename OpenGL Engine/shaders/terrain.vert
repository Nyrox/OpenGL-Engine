#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 tangent;

out vec3 fragPos;
out vec3 surfaceNormal;
out vec2 uv;

out VS_OUT {
	mat3 TBN;
} vs_out;

uniform float uvScale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 dir_light_space_matrix_0;
out vec4 frag_pos_dir_light_space_0;

#include("headers/common.glsl");

void main() {
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);
	surfaceNormal = mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(model * vec4(position, 1.0));

	uv = UV * vec2(uvScale, uvScale);

	frag_pos_dir_light_space_0 = dir_light_space_matrix_0 * vec4(fragPos, 1.0);	

	vs_out.TBN = createTBNMatrix(model, normal, tangent);
}