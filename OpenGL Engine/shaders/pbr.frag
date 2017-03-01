#version 420 core

in VS_OUT {
	vec3 faceNormal;
	vec3 fragPos;
	vec2 uv;

	mat3 TBN;
} fs_in;

out vec4 fragColor;


struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 camera_position;

uniform PointLight point_lights[16];
uniform int point_light_count;

uniform samplerCube shadow_map_0;



uniform sampler2D tex2D_albedo;
uniform sampler2D tex2D_roughness;
uniform sampler2D tex2D_normal;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main() {
	vec3 N = normalize(texture(tex2D_normal, fs_in.uv).rgb * 2.0 - 1.0);
	N = normalize(fs_in.TBN * N);

	vec3 V = normalize(camera_position - fs_in.fragPos);

	const vec3 albedo = texture(tex2D_albedo, fs_in.uv).rgb;
	float roughness = texture(tex2D_roughness, fs_in.uv).r;

	const float metallic = 1.0;

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
    for(int i = 0; i < 2; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(point_lights[i].position - fs_in.fragPos);
        vec3 H = normalize(V + L);
        float distance    = length(point_lights[i].position - fs_in.fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = point_lights[i].diffuse * 20 * attenuation;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
        vec3 brdf = nominator / denominator;
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + brdf) * radiance * NdotL; 
    }   


	fragColor = vec4(Lo, 1.0);
}