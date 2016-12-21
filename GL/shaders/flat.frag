#version 420 core;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 color;

uniform sampler2D diffuse;

void main() {
	vec4 diffuse = texture(diffuse, uv);
	if(diffuse.a == 0) {
		discard();
	}

	color = diffuse;
}