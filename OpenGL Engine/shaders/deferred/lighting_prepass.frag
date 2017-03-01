#version 420 core

#include("headers/lighting.glsl");

layout (location = 0) out vec4 albedoSpec;

in VS_OUT {
	vec2 uv;
} fs_in;



struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight point_lights[16];


uniform vec3 camera_position;

uniform sampler2D tex2D_geoPositions;
uniform sampler2D tex2D_geoNormals;

void main() {
	vec4 result;

	for(int i = 0; i < 2; i++) {
		vec3 fragmentNormal = texture(tex2D_geoNormals, fs_in.uv).rgb;
		vec3 fragmentPosition = texture(tex2D_geoPositions, fs_in.uv).rgb;

		vec3 lightDir = normalize(point_lights[i].position - fragmentPosition);
		vec3 viewDir = normalize(camera_position - fragmentPosition);

		float diffuse = max(dot(fragmentNormal, lightDir), 0.0);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		
		float distance = length(point_lights[i].position - fragmentPosition);
		float attenuation = calculateAttenuation(distance, point_lights[i].constant, point_lights[i].linear, point_lights[i].quadratic);

		result.rgb += attenuation * point_lights[i].diffuse * diffuse;		
	}

	albedoSpec = vec4(result.rgb, 1.0);
}