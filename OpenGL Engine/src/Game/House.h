#pragma once
#include <Core\Model.h>
#include <Core\Physics\AABB.h>
#include <Core\SceneNode.h>
#include <Core\Material.h>
#include <Core\Renderer.h>

#include <functional>

namespace {
	Material C_HouseMaterial = Material(Material::ShadingModel::PBR);
}

class House : public SceneNode {
public:
	explicit House(Renderer& renderer) : model(C_HouseMaterial, std::make_shared<Mesh>("assets/house.ply")) {

	}


	virtual Physics::AABB getSceneBoundingBox() const override {
		return collision;
	}

	Model model;
	Physics::AABB collision;
};
