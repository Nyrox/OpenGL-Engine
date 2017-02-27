#include "Gizmo.h"

Gizmo::Gizmo() {
	shader.loadFromFile("shaders/basic.vert", "shaders/uniformColor.frag");
	translationGizmo.loadFromFile("assets/translation_gizmo.ply");
}


void Gizmo::render() const {
	if (sceneNode == nullptr) return;

	gl::Disable(gl::DEPTH_TEST);

	shader.bind();

	translationGizmo.draw();
}