#version 430 core

in vec2 UV;

out vec4 color;

uniform sampler2D font;
uniform vec3 fillColor;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(font, UV).r);
	color = vec4(fillColor, 1.0) * sampled;
	//color = vec4(1.0, 0.0, 0.0, 1.0);
}

