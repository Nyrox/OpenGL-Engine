const float PI = 3.14159265359;

struct PbrMaterial {
	sampler2D albedo;
	sampler2D roughness;
	sampler2D metal;
	sampler2D normal;
};

float calculateAttenuation(float distance, float constant, float linear, float quadratic) {
	return 1.0f / (constant + linear * distance + quadratic * (distance * distance));
}

/*
	Calculate the ratio of diffuse versus specular lighting
	CosTheta: The dot product of the normal of the fragment and a direction vector from the eye to the fragment
	F0: The approximated base reflectivity of the surface
*/
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

/*
	Same as fresnelSchlick, but specialized to take roughness into account for IBL
*/
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

/*
	Calculate the ratio of microfacets which normals are aligned with some halfway vector H
*/
float GGX_NormalDistribution(vec3 normal, vec3 halfwayVector, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;

	float NdotH = max(dot(normal, halfwayVector), 0.0); 
	float NdotH2 = NdotH * NdotH;

	float divisor = (NdotH2 * (a2 - 1.0) + 1.0);
	divisor = PI * divisor * divisor;

	return a2 / divisor;
}


/*
	Helper function for SmithMicrofacetSelfShadowing
*/
float impl_GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}

/*
	Calculate the extent to which microfacets are self shadowing each other
*/
float SmithMicrofacetSelfShadowing(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);

	float ggx2 = impl_GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = impl_GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}