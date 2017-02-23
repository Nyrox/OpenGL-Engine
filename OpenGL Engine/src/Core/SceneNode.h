#pragma once
#include <Core\Transform.h>

#include <vector>
#include <memory>

namespace Physics {
	class AABB;
}

class SceneNode {
public:
	std::vector<std::unique_ptr<SceneNode>> children;

	// World space transform
	Transform transform;

	/*
		Requires SceneNode's to implement a method to generate a bounding box used for selections in the Editor/Ingame-Debugger
	*/
	virtual Physics::AABB getSceneBoundingBox() = 0;
private:


};