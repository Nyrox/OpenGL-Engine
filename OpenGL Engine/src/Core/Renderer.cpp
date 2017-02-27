#include "Renderer.h"
#include <GLFW\glfw3.h>
#include <algorithm>
#include <iostream>

Renderer::Renderer(Camera& t_camera, float backbuffer_width, float backbuffer_height) : camera(t_camera), postProcessTexture(false, gl::CLAMP_TO_BORDER, gl::NEAREST)
	//: postProcessBuffer({ 1280, 720 }, RGB) 
{
	shadow_pass_shader.loadFromFile("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", "shaders/shadow_pass.geom");
	dirLightShadowPassShader.loadFromFile("shaders/directional_shadow_pass.vert", "shaders/directional_shadow_pass.frag");
	forward_render_shader.loadFromFile("shaders/basic.vert", "shaders/basic.frag");
	post_process_shader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");

	skybox.loadFromFiles({
		"assets/skybox/blue_rt.tga",
		"assets/skybox/blue_lf.tga",
		"assets/skybox/blue_up.tga",
		"assets/skybox/blue_dn.tga",
		"assets/skybox/blue_bk.tga",
		"assets/skybox/blue_ft.tga"
	});

	postProcessTexture.allocate(gl::RGB16F, { 1280, 720 });
	postProcessDepthTexture.allocate(gl::DEPTH24_STENCIL8, { 1280, 720 });
	postProcessBuffer.attach(gl::COLOR_ATTACHMENT0, postProcessTexture);
	postProcessBuffer.attach(gl::DEPTH_STENCIL_ATTACHMENT, postProcessDepthTexture);
}

void Renderer::insert(Model* model) {
	if (model->material.shadingModel == Material::ShadingModel::Opague) {
		this->opagues.push_back(model);
	} 
	else if (model->material.shadingModel == Material::ShadingModel::Transparent) {
		this->transparents.push_back(model);
	}
}

void Renderer::addPointLight(PointLight light) {
	this->point_lights.push_back(light);
	this->shadow_maps.emplace_back(glm::vec2{ 1024, 1024 }, CUBE_DEPTH);

}

void Renderer::addDirectionalLight(DirectionalLight light) {
	this->directional_lights.push_back(light);
	this->directional_shadow_maps.emplace_back(glm::vec2(1024, 1024), DEPTH);
}

// Far and near planes for point lights.
constexpr float POINT_LIGHT_DEPTH_MAP_NEAR_PLANE = 0.1f;
constexpr float POINT_LIGHT_DEPTH_MAP_FAR_PLANE = 100.f;

constexpr float DIRECTIONAL_LIGHT_DEPTH_MAP_NEAR_PLANE = 10.f;
constexpr float DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE = 300.f;

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

		for (auto& it : opagues) {
			shadow_pass_shader.setUniform("model", it->transform.getModelMatrix());
			shadow_pass_shader.setUniform("material", it->material);

			it->mesh->draw();
		}
	}

	dirLightShadowPassShader.bind();
	dirLightShadowPassShader.setUniform("far_plane", DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE);
	for (int i = 0; i < directional_lights.size(); i++) {
		DirectionalLight& light = directional_lights.at(i);
		Framebuffer& fb = directional_shadow_maps.at(i);

		fb.bind();
		gl::Viewport(0, 0, fb.width, fb.height);
		gl::Clear(gl::DEPTH_BUFFER_BIT);

		glm::mat4 shadow_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, DIRECTIONAL_LIGHT_DEPTH_MAP_NEAR_PLANE, DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE);
		glm::mat4 shadow_view = glm::lookAt(-light.direction * 100.f, { 0, 0, 0, }, { 0, 1, 0 });

		dirLightShadowPassShader.setUniform("lightSpaceMatrix", shadow_projection * shadow_view);

		for (auto& it : opagues) {
			dirLightShadowPassShader.setUniform("model", it->transform.getModelMatrix());
			dirLightShadowPassShader.setUniform("material", it->material);

			it->mesh->draw();
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

	skybox.render(camera.projection, camera.getViewMatrix());


	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::BACK);
	gl::FrontFace(gl::CCW);

	
	auto setUniforms = [&](Shader& shader) {
		shader.setUniform("camera_position", camera.transform.position);
		shader.setUniform("view", camera.getViewMatrix());
		shader.setUniform("projection", camera.projection);
		shader.setUniform("shadow_far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);
		
		
		shader.setUniform("point_light_count", (int)point_lights.size());
		for (int i = 0; i < point_lights.size(); i++) {
			shader.setUniform("point_lights[" + std::to_string(i) + "]", point_lights.at(i));
			shader.setUniform("shadow_map_" + std::to_string(i), 5 + i);
			shadow_maps.at(i).bindTexture(5 + i);
		}

		shader.setUniform("directional_light_count", (int)directional_lights.size());
		for (int i = 0; i < directional_lights.size(); i++) {
			shader.setUniform("directional_lights[" + std::to_string(i) + "]", directional_lights.at(i));
			shader.setUniform("dir_shadow_map_" + std::to_string(i), 21 + i);
			directional_shadow_maps.at(i).bindTexture(21 + i);
			
			Framebuffer& fb = directional_shadow_maps.at(i);
			DirectionalLight& light = directional_lights.at(i);

			glm::mat4 shadow_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, DIRECTIONAL_LIGHT_DEPTH_MAP_NEAR_PLANE, DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE);
			glm::mat4 shadow_view = glm::lookAt(-light.direction * 100.f, { 0, 0, 0, }, { 0, 1, 0 });

			shader.setUniform("dir_light_space_matrix_" + std::to_string(i), shadow_projection * shadow_view);
		}
	};

	
	// Draw opagues
	for (auto& it : opagues) {
		Shader& shader = it->material.shader;
		shader.bind();
		setUniforms(shader);

		it->material.diffuse->bind(0);

		if		(std::holds_alternative<glm::vec3>(it->material.specular))  { std::get<glm::vec3>(it->material.specular); }
		else if (std::holds_alternative<Texture2D*>(it->material.specular)) { std::get<Texture2D*>(it->material.specular)->bind(1); }

		if (it->material.normal != nullptr) {
			it->material.normal->bind(4);
			shader.setUniform("useNormalMap", true);
			shader.setUniform("normalMap", 4);
		}
		else {
			shader.setUniform("useNormalMap", false);
		}

		shader.setUniform("model", it->transform.getModelMatrix());
		shader.setUniform("material", it->material);

		it->mesh->draw();
		
	}

	std::sort(transparents.begin(), transparents.end(), [&](const Model* a, const Model* b) {
		return glm::distance(camera.transform.position, a->transform.position) > glm::distance(camera.transform.position, b->transform.position);
	});

	gl::Enable(gl::BLEND);
	// Draw transparents
	for (auto& it : transparents) {
		Shader& shader = it->material.shader;
		shader.bind();
		it->material.diffuse->bind(0);
		//it.material.specular.bind(1);
		
		shader.setUniform("model", it->transform.getModelMatrix());
		shader.setUniform("material", it->material);
		
		it->mesh->draw();
	}

#ifdef GL_WIREFRAME
	gl::PolygonMode(gl::FRONT_AND_BACK, gl::FILL);
#endif
	gl::Disable(gl::DEPTH_TEST);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//gl::Clear(gl::COLOR_BUFFER_BIT);

	post_process_shader.bind();
	post_process_shader.setUniform("screen_capture", 0);
	//postProcessBuffer.bindTexture(0);
	postProcessTexture.bind(0);

	Mesh canvas = Mesh::generatePlane({ 2, 2 });
	canvas.draw();
}