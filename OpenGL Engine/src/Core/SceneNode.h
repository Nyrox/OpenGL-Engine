#pragma once
#include <vector>
#include <memory>

struct AABB;
class SceneNode {
public:
	std::vector<std::unique_ptr<SceneNode>> children;


	/*
		Requires SceneNode's to implement a method to generate a bounding box used for selections in the Editor/Ingame-Debugger
	*/
	virtual AABB getSceneBoundingBox() = 0;
private:


};