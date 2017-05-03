#pragma once
#include <Core/Definitions.h>

class GameObject;

class Component {
public:
	Component(GameObject& t_go) : gameObject(t_go) { }

	GameObject& gameObject;
};
