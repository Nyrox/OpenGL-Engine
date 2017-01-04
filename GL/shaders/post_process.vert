#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 UV;

out vec2 uv;

void main()
{
    gl_Position = vec4(position.x - 1, position.y - 1, 0.0f, 1.0f);
	uv = UV;
}