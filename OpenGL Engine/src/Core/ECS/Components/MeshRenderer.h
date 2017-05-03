#pragma once
#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/ECS/Component.h>

class MeshRenderer : public Component {
public:
	MeshRenderer(GameObject& t_go, Mesh* t_mesh, Material* t_material) : Component(t_go), mesh(t_mesh), material(t_material) { }


	Mesh* mesh;
	Material* material;
};