#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;


out VS_OUT {
	vec3 world_fragPos;
	vec3 world_surfaceNormal;
	vec2 uv;
	mat3 TBN;
} vs_out;

uniform float uvScale = 1.f;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat3 createTBNMatrix(mat4 model, vec3 normal, vec3 tangent) {
	vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
	vec3 B = cross(T, N);

	return mat3(T, B, N);
}

void main() {
	gl_Position = projection * view * model * vec4(position.xyz, 1.0);
	
	vs_out.world_fragPos = vec3(model * vec4(position, 1.0));
	vs_out.world_surfaceNormal = mat3(transpose(inverse(model))) * normal;
	vs_out.uv = uv;
	vs_out.TBN = createTBNMatrix(model, normal, tangent);
}