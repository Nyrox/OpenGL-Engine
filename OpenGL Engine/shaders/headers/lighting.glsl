

float calculateAttenuation(float distance, float constant, float linear, float quadratic) {
	return 1.0f / (constant + linear * distance + quadratic * (distance * distance));
}