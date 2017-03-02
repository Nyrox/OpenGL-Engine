#pragma once
#include <Core\Transform.h>

#include <vector>
#include <memory>

namespace Physics {
	class AABB;
}


class SceneNode {
public:
	SceneNode(Transform transform = Transform());
	virtual ~SceneNode() = default;

	std::vector<std::unique_ptr<SceneNode>> children;

	// World space transform
	Transform transform;
	// Scene this node is attached to
	class Scene* scene = nullptr;

	/*
		Requires SceneNode's to implement a method to generate a bounding box used for selections in the Editor/Ingame-Debugger
	*/
	virtual Physics::AABB getSceneBoundingBox() const = 0;
private:


};