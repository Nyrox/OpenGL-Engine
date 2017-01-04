#version 330 core
in vec2 uv;
out vec4 color;

uniform sampler2D screen_capture;

void main() {
	vec3 result = texture(screen_capture, uv).rgb;

	color = vec4(result, 1.0);


	//color = visualizeDepth();
}