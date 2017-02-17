#version 330 core
in vec2 uv;
out vec4 color;

uniform sampler2D screen_capture;

void main() {

	const float exposure = 1.f;
	const float gamma = 2.2;

    vec3 hdrColor = texture(screen_capture, uv).rgb;
  
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    color = vec4(mapped, 1.0);
}