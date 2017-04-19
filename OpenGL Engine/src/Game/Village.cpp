#include "Village.h"
#include <Core\Renderer.h>
#include <Core\Physics\AABB.h>
#include <Game\House.h>

using namespace Physics;

Village::Village(Renderer& renderer) {
	this->children.push_back(std::make_unique<House>(renderer));
}

AABB Village::getSceneBoundingBox() const {
	return AABB();
}