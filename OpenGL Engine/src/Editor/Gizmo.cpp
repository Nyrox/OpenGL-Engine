#include "Gizmo.h"
#include <iostream>

using namespace Physics;

Gizmo::Gizmo() {
	shader.loadFromFile("shaders/basic.vert", "shaders/uniformColor.frag");
	translationGizmo.loadFromFile("assets/translation_gizmo.ply");

	
}

void Gizmo::update() {
	if (!sceneNode) return;

	xBounds = AABB(sceneNode->transform.position + glm::vec3(0.7, -0.1, -0.1), sceneNode->transform.position + glm::vec3(1, 0.1, 0.1));
	yBounds = AABB(sceneNode->transform.position + glm::vec3(-0.1, 0.7, -0.1), sceneNode->transform.position + glm::vec3(0.1, 1, 0.1));
	zBounds = AABB(sceneNode->transform.position + glm::vec3(-0.1, -0.1, 0.7), sceneNode->transform.position + glm::vec3(0.1, 0.1, 1));
}

void Gizmo::render(const glm::mat4& view, const glm::mat4& projection) const {
	if (sceneNode == nullptr) return;

	gl::Disable(gl::DEPTH_TEST);

	shader.bind();
	shader.setUniform("projection", projection);
	shader.setUniform("view", view);

	shader.setUniform("model", sceneNode->transform.getModelMatrix());
	shader.setUniform("fillColor", glm::vec3(0, 1, 0));
	translationGizmo.draw();

	shader.setUniform("model", glm::rotate(sceneNode->transform.getModelMatrix(), glm::radians(90.f), { 0, 0, -1 }));
	shader.setUniform("fillColor", glm::vec3(1, 0, 0));
	translationGizmo.draw();

	shader.setUniform("model", glm::rotate(sceneNode->transform.getModelMatrix(), glm::radians(90.f), { 1, 0, 0 }));
	shader.setUniform("fillColor", glm::vec3(0, 0, 1));
	translationGizmo.draw();
}

bool Gizmo::handleEvent(const Event& event) {
	static bool isDragging;
	static glm::vec3 axis;
	static glm::vec3 translationStart;
	static glm::vec3 originalPosition;

	switch (event.type) {
	case Event::MouseDown:
	{
		Ray ray = Physics::screenPositionToRay(*event.click.camera, { event.click.x, event.click.y });
		glm::vec3 q;
		float t;
		std::cout << "Translating x: " << xBounds.intersects(ray, &t, &q) << "\n";
		std::cout << "Translating y: " << yBounds.intersects(ray, &t, &q) << "\n";
		std::cout << "Translating z: " << zBounds.intersects(ray, &t, &q) << "\n";


		if (xBounds.intersects(ray, &t, &q)) {
			isDragging = true;
			axis = { 1, 0, 0 };
			translationPlane = Plane(sceneNode->transform.position, { 0, 1, 0 });
		}

		if (yBounds.intersects(ray, &t, &q)) {
			isDragging = true;
			axis = { 0, 1, 0 };
			translationPlane = Plane(sceneNode->transform.position, { 0, 0, 1 });
		}

		if (zBounds.intersects(ray, &t, &q)) {
			isDragging = true;
			axis = { 0, 0, 1 };
			translationPlane = Plane(sceneNode->transform.position, { 0, 1, 0 });
		}

		if (isDragging == true) {
			translationPlane.intersects(ray, t, q);
			translationStart = q;
			originalPosition = sceneNode->transform.position;
			return false;
		}

		break;
	}
	case Event::MouseUp:
		isDragging = false;
		break;
	case Event::MouseMove:
	{
		if (!isDragging) return true;

		glm::vec3 q; 
		float t;

		if (!translationPlane.intersects(screenPositionToRay(*event.mouse.camera, { event.mouse.x, event.mouse.y }), t, q)) {
			std::cout << "Gizmo translation plane is not intersecting ray!!!" << "\n";
			return false;
		}
		
		glm::vec3 translationOffset = (q - translationStart) * axis;
		sceneNode->transform.position = originalPosition + translationOffset;

		return false;
		break;
	}
	}

	return true;
}