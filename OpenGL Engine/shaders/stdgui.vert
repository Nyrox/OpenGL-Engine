#version 420 core

layout(location = 0) in vec3 position;

uniform mat4 view;

void main() {
	gl_Position = view * vec4(position.x, position.y, 0, 1.0);
}