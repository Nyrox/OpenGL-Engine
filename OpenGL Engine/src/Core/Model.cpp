#include "Model.h"

Model::Model(Material& t_material, std::shared_ptr<Mesh> t_mesh, Transform t_transform) 
	: material(t_material), mesh(t_mesh), transform(t_transform) {

}