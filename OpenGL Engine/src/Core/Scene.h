#pragma once
#include "SceneNode.h"

class Scene {
public:



	// Scene acts as the root node 
	std::vector<std::unique_ptr<SceneNode>> nodes;
};