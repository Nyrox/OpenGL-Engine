#pragma once
#include <Core/Transform.h>
#include <Core/ECS/Component.h>

#include <vector>
#include <memory>
#include <unordered_map>

namespace Physics {
	class AABB;
}

class Engine;

class GameObject {
public:
	GameObject(Engine& t_engine, Transform t_transform = Transform()) : engine(t_engine), transform(t_transform) { };
	GameObject(const GameObject&) = default;
	GameObject& operator=(const GameObject&) = default;
	
	Physics::AABB getSceneBoundingBox() const;

	template<class T>
	T* getComponent() const {
		for (Component* c : components) {
			if (c->getTypeIndex() == typeid(T).hash_code()) {
				return static_cast<T*>(c);
			}
		}

		return nullptr;
	}


	void setParent(GameObject* parent);
	void addComponent(Component* component);
	
	GameObject* getParent() const { return parent; }
	

	Engine& engine;
	Transform transform;
private:
	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};