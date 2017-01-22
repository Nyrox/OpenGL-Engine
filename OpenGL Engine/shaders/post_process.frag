#version 330 core
in vec2 uv;
out vec4 color;

uniform sampler2D screen_capture;

void main() {
	vec3 result = texture(screen_capture, uv).rgb;



	float gamma = 2.2;
    color.rgb = pow(result, vec3(1.0/gamma));
	color.a = 1.0;

}