#pragma once
#include <Core\Mesh.h>
#include <Core\Material.h>
#include <Core\Transform.h>

#include <memory>

class Model {
public:

	Material material;
	Transform transform;
	std::shared_ptr<Mesh> mesh;
private:


};

