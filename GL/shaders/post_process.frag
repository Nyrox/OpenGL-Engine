#version 330 core
in vec2 uv;
out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;

uniform float near_plane;
uniform float far_plane;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

vec4 visualizeDepth() {
	float depth = texture(screenTexture, uv).r;
	depth = linearizeDepth(depth) / far_plane;
    return vec4(vec3(depth), 1.0);
}

void main() {
	vec4 result = texture(screenTexture, uv);

	color = result;	


	//color = visualizeDepth();
}