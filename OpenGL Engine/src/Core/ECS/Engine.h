#pragma once
#include <Core/Definitions.h>
#include <unordered_map>
#include <Core/Libs/colony.h>

class Component;

class Engine {	
public:
	friend class GameObject;

	template<class T, class... Args>
	T* emplaceComponent(Args&&... args) {
		return (T*)components[typeid(T).hash_code()].insert(std::make_unique<T>(args...))->get();
	}

	template<class T>
	plf::colony<uptr<Component>>& getAllComponentsOfType() {
		return components[typeid(T).hash_code()];
	}

private:
	std::unordered_map<uint32, plf::colony<uptr<Component>>> components;

};