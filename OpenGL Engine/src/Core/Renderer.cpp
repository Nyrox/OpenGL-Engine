#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <ImmediateDraw.h>
#include <algorithm>

Renderer::Renderer(float backbuffer_width, float backbuffer_height) : postProcessBuffer({ backbuffer_width, backbuffer_height }, RGB) {
	shadow_pass_shader.loadFromFile("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", "shaders/shadow_pass.geom");
	forward_render_shader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");
	post_process_shader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");
}

void Renderer::addPointLight(PointLight light) {
	this->point_lights.push_back(light);
	this->shadow_maps.emplace_back(glm::vec2{ 1024, 1024 }, CUBE_DEPTH);

}

// Far and near planes for point lights.
constexpr float POINT_LIGHT_DEPTH_MAP_NEAR_PLANE = 0.1f;
constexpr float POINT_LIGHT_DEPTH_MAP_FAR_PLANE = 100.f;

//#define GL_WIREFRAME

void Renderer::render() {

	for (auto& it : point_lights) {
		it.position.z += std::sin(glfwGetTime()) / 15;
	}

	// Render shadow maps
	shadow_pass_shader.bind();
	shadow_pass_shader.setUniform("far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

	gl::Enable(gl::DEPTH_TEST);
	gl::Disable(gl::BLEND);

	gl::DepthFunc(gl::LESS);
	for (int i = 0; i < point_lights.size(); i++) {
		PointLight& light = point_lights.at(i);
		Framebuffer& fb = shadow_maps.at(i);

		fb.bind();
		gl::Viewport(0, 0, fb.width, fb.height);
		gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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


#ifdef GL_WIREFRAME
	gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE);
#endif
	// Render scene
	postProcessBuffer.bind();
	//gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	gl::Viewport(0, 0, 1280, 720);
	gl::ClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::CCW);

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
	
	// Draw opagues
	for (auto& it : meshes) {
		it.material.diffuse.bind(0);
		it.material.specular.bind(1);

		it.draw(forward_render_shader);
	}


	transparent.sort([&](const Mesh& a, const Mesh& b) {
		return glm::distance(camera->position, a.position) > glm::distance(camera->position, b.position);
	});

	// Draw transparents
	for (auto& it : transparent) {
		it.material.diffuse.bind(0);
		it.material.specular.bind(1);

		it.draw(forward_render_shader);
	}

#ifdef GL_WIREFRAME
	gl::PolygonMode(gl::FRONT_AND_BACK, gl::FILL);
#endif
	gl::Disable(gl::DEPTH_TEST);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT);

	post_process_shader.bind();
	post_process_shader.setUniform("screen_capture", 0);
	postProcessBuffer.bindTexture(0);


	ImmediateDraw::drawPlane(2, 2);
	
}