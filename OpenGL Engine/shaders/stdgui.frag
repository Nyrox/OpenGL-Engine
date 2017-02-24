#version 420 core

out vec4 color;

uniform vec3 fillColor;

void main() {
	color = vec4(fillColor, 1.0);
}