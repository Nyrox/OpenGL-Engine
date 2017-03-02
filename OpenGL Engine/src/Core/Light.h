#pragma once
#include <Core/Framebuffer.h>
#include <Core/SceneNode.h>

#include <gl_core_4_3.hpp>
#include <glm/glm.hpp>

class PointLight : public SceneNode {
public:
	PointLight(Transform transform = Transform(), uint32_t shadowMapResolution = 1024, glm::vec3 t_color = glm::vec3(1), float t_intensity = 1.0, float t_ambient = 0.15)
		: SceneNode(transform), color(t_color), intensity(t_intensity), ambientIntensity(t_ambient) {
		
		setEffectiveRange(100);
	}
	
	/*
	Calculates the constant, linear and quadratic components of a light based on range.
	It should be noted that this is a rather costly operation and shouldn't be done too lightly.
	*/
	void setEffectiveRange(float range);
	virtual Physics::AABB getSceneBoundingBox() const override;

	Refactor::Framebuffer shadowMapBuffer;

	glm::vec3 color;
	float ambientIntensity;
	float intensity;


	float constant, linear, quadratic;	
};

struct DirectionalLight {

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};