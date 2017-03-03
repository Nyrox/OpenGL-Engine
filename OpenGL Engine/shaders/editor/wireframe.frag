#version 420 core

in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 outFragColor;

void main() {
	outFragColor = vec4(1.0, 0, 0, 1.0);
}