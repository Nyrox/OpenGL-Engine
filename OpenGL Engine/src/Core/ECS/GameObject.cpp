#include "GameObject.h"
#include <algorithm>

void GameObject::setParent(GameObject* nP) {
	if (parent) {
		std::remove(parent->children.begin(), parent->children.end(), this);
	}

	parent = nP;

	if (std::find(parent->children.begin(), parent->children.end(), this) == parent->children.end()) {
		parent->children.push_back(this);
	}
}

void GameObject::addComponent(Component* component) {
	components.push_back(component);
}