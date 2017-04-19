#pragma once
#include <Core\SceneNode.h>
#include <array>



class Village : public SceneNode {
public:
	Village(class Renderer& renderer); 

	Physics::AABB getSceneBoundingBox() const override;

	uint32_t wood = 125;
	uint32_t stone = 25;

	std::string name;
private:

};