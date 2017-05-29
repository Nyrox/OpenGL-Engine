#pragma once
#include <Core/ECS/Component.h>
#include <glm/glm.hpp>

class PointLight : public Component {
public:
	PointLight(GameObject& go, float effectiveRange = 10) : Component(go) {
		setEffectiveRange(5);
	}

	void setEffectiveRange(float range);

	glm::vec3 color;
	float intensity = 1;
	float constant, linear, quadratic;
};