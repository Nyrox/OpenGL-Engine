#version 420 core

in VS_OUT {
	vec2 uv;
} fs_in;


out vec4 fragColor;

uniform sampler2D tex2D_albedoSpec;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

void main() {
	
	vec2 screenSpaceUV = vec2(gl_FragCoord.x / 1280, gl_FragCoord.y / 720);

	vec3 diffuseLight = texture(tex2D_albedoSpec, screenSpaceUV).rgb;
	float specularIntensity = texture(tex2D_albedoSpec, screenSpaceUV).a;

	vec3 diffuse = diffuseLight * vec3(texture(material.diffuse, fs_in.uv)).rgb;
	vec3 specular = specularIntensity * vec3(texture(material.diffuse, fs_in.uv)).rgb;

	vec3 lighting = diffuse + specular;

	fragColor = vec4(lighting, 1.0);
}
