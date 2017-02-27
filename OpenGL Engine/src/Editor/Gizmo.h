#pragma once
#include <Core/Mesh.h>
#include <Core/SceneNode.h>
#include <Core/Shader.h>
#include <Core/Event.h>
#include <Core\Physics\AABB.h>
#include <Core\Physics\Plane.h>

class Gizmo {
public:
	Gizmo();

	void update();
	void render(const glm::mat4& view, const glm::mat4& projection) const;
	void handleEvent(const Event& event);
	
	SceneNode* sceneNode;
private:
	Shader shader;
	Mesh translationGizmo;

	Physics::AABB xBounds;
	Physics::AABB yBounds;
	Physics::AABB zBounds;

	Physics::Plane translationPlane;
};