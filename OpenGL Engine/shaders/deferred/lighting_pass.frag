#version 420 core

#include("headers/lighting.glsl");

in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 fragColor;

uniform sampler2D tex2D_radiance;
uniform sampler2D tex2D_brdf;
uniform sampler2D tex2D_Kd;

void main() {
	vec2 screenSpaceUv = vec2(gl_FragCoord.x / 1280, gl_FragCoord.y / 720);

	vec3 color = texture(tex2D_radiance, screenSpaceUv).rgb * (texture(tex2D_Kd, screenSpaceUv).rgb + texture(tex2D_brdf, screenSpaceUv).rgb);
	fragColor = vec4(color, 1.0);
}