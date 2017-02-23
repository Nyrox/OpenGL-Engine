#pragma once
#include "SceneNode.h"

class Scene {
public:


	template<class T = SceneNode>
	T* insert(std::unique_ptr<T>&& node);

	template<class T = SceneNode, class... Args>
	T* emplace(Args&&... args);
private:

	// Scene acts as the root node 
	std::vector<std::unique_ptr<SceneNode>> nodes;
};

template<class T>
T* Scene::insert(std::unique_ptr<T>&& node) {
	node->scene = this;
	SceneNode* returnPtr = node.get();

	this->nodes.push_back(std::move(node));
	return dynamic_cast<T*>(returnPtr);
}

template<class T = SceneNode, class... Args>
T* Scene::emplace(Args&&... args) {
	this->nodes.emplace_back(std::make_unique<T>(args...));

	SceneNode* node = this->nodes.back().get();
	node->scene = this;
	return dynamic_cast<T*>(node);
}