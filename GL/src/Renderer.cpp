#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ImmediateDraw.h>

Renderer::Renderer() : postProcessBuffer({ 800, 600 }, RGB) {
	shadow_pass_shader.loadFromFile("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", "shaders/shadow_pass.geom");
	forward_render_shader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");
	post_process_shader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");
}

void Renderer::addPointLight(PointLight light) {
	this->point_lights.push_back(light);
	this->shadow_maps.emplace_back(glm::vec2{ 1024, 1024 }, CUBE_DEPTH);

}

// Far and near planes for point lights.
// Should eventually be tight to light range for performance reasons
const float POINT_LIGHT_DEPTH_MAP_NEAR_PLANE = 0.1f;
const float POINT_LIGHT_DEPTH_MAP_FAR_PLANE = 100.f;

void Renderer::render() {

	for (auto& it : point_lights) {
		it.position.z += std::sin(glfwGetTime()) / 15;
	}

	// Render shadow maps
	shadow_pass_shader.bind();
	shadow_pass_shader.setUniform("far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);
	for (int i = 0; i < point_lights.size(); i++) {
		PointLight& light = point_lights.at(i);
		Framebuffer& fb = shadow_maps.at(i);

		fb.bind();
		gl::Viewport(0, 0, fb.width, fb.height);
		gl::Clear(gl::DEPTH_BUFFER_BIT);
	
		shadow_pass_shader.setUniform("light_position", light.position);

		glm::mat4 shadow_projection = glm::perspective(glm::radians(90.f), 1.f, POINT_LIGHT_DEPTH_MAP_NEAR_PLANE, POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		shadow_pass_shader.setUniformArray("shadowMatrices", shadowTransforms.data(), shadowTransforms.size());

		for (auto& it : meshes) {
			it.draw(shadow_pass_shader);
		}
	}


	// Render scene
	postProcessBuffer.bind();
	gl::Viewport(0, 0, 800, 600);
	gl::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	
	forward_render_shader.bind();
	forward_render_shader.setUniform("camera_position", camera->position);
	forward_render_shader.setUniform("view", camera->getViewMatrix());
	forward_render_shader.setUniform("projection", projection);
	forward_render_shader.setUniform("shadow_far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);
	
	forward_render_shader.setUniform("point_light_count", (int)point_lights.size());
	shadow_maps.at(0).bindTexture(7);
	
	//forward_render_shader.setUniform("shadow_map_2", 5);

	for (int i = 0; i < point_lights.size(); i++) {
		
		forward_render_shader.setUniform("point_lights[" + std::to_string(i) + "]", point_lights.at(i));
		// Reserve 4 texture units for materials
		forward_render_shader.setUniform("shadow_map_" + std::to_string(i), 5 + i);
		shadow_maps.at(i).bindTexture(5 + i);
	}
	
	// TODO: Add texture binding
	for (auto& it : meshes) {
		it.draw(forward_render_shader);
	}

	//gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE);
	gl::Disable(gl::DEPTH_TEST);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	post_process_shader.bind();
	post_process_shader.setUniform("screen_capture", 0);
	postProcessBuffer.bindTexture(0);


	ImmediateDraw::drawPlane(2, 2);
}