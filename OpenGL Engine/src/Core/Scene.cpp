#include "Scene.h"
#include <Core/Physics/Ray.h>
#include <Core/Physics/AABB.h>
#include <algorithm>

using namespace Physics;


Scene::Scene() : wireframeShader("shaders/editor/wireframe.vert", "shaders/editor/wireframe.frag"), wireframeMaterial(Material::ShadingModel::PBR), sceneCollisionDebugModel(wireframeMaterial, std::make_shared<Mesh>(Mesh::generateCube({ 1, 1, 1 }))) {

}

std::vector<std::pair<float, SceneNode*>> Scene::raycastAgainstSceneCollision(const Ray& ray) {
	std::vector<std::pair<float, SceneNode*>> results;

	float t;
	glm::vec3 q;

	for (auto& it : nodes) {
		if (it->getSceneBoundingBox().intersects(ray, &t, &q)) {
			results.push_back(std::make_pair(glm::distance(ray.origin, q), it.get()));
		}
	}

	std::sort(results.begin(), results.end(), [&](const std::pair<float, SceneNode*>& left, const std::pair<float, SceneNode*>& right) {
		return std::get<float>(left) > std::get<float>(right);
	});
	return results;
}

void Scene::update() {

}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
	Shader& shader = wireframeShader;
	shader.bind();

	gl::Disable(gl::DEPTH_TEST);
	gl::Enable(gl::CULL_FACE);
	gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE);

	shader.setUniform("view", view);
	shader.setUniform("projection", projection);

	for (auto& it : nodes) {
		AABB& bb = it->getSceneBoundingBox();
		shader.setUniform("model", glm::scale(glm::translate(bb.min), bb.max - bb.min));
		
		sceneCollisionDebugModel.mesh->draw();
	}

	gl::PolygonMode(gl::FRONT_AND_BACK, gl::FILL);
}