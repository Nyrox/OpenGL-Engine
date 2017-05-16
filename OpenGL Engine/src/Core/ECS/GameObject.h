#pragma once
#include <Core/Transform.h>
#include <Core/ECS/Component.h>
#include <Core/Engine.h>

#include <vector>
#include <memory>
#include <unordered_map>

namespace Physics {
	class AABB;
}


class GameObject {
public:
	GameObject(Engine& t_engine, Transform t_transform = Transform()) : engine(t_engine), transform(t_transform) { };
	GameObject(const GameObject&) = default;
	GameObject& operator=(const GameObject&) = default;
	
	Physics::AABB getSceneBoundingBox() const;

	template<class T>
	T* getComponent() const {
		return static_cast<T*>(components.at(typeid(T).hash_code()));
	}

	template<class T, class... Args>
	void addComponent(Args&&... args) {
		components.emplace(typeid(T).hash_code(), engine.emplaceComponent<T>(args...));
	}


	Engine& engine;
	Transform transform;
	std::vector<GameObject> children;

private:
	std::unordered_map<uint32, Component*> components;
};