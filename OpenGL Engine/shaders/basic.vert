#version 420 core

/*
	Basic vertex shader
*/

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 dir_light_space_matrix_0;
out vec4 frag_pos_dir_light_space_0;

void main() {
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);
	normal = mat3(transpose(inverse(model))) * Normal;
	fragPos = vec3(model * vec4(position, 1.0));
	uv = UV * 8;
	frag_pos_dir_light_space_0 = dir_light_space_matrix_0 * vec4(fragPos, 1.0);	
}