#include "Renderer.h"
#include <GLFW\glfw3.h>
#include <algorithm>
#include <iostream>
#include <Core/Engine.h>
#include <Core/ECS/GameObject.h>
#include <Core/ECS/Components/MeshRenderer.h>

#include <Core/glUtil.h>
#include <Core/ECS/Components/PointLight.h>

Renderer::Renderer(Engine& t_engine, Camera& t_camera, float backbuffer_width, float backbuffer_height) : engine(t_engine), camera(t_camera), postProcessTexture(TextureSettings(NoMipmaps, ClampToBorder, Nearest)) {
	shadowPassShader.loadFromFile("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", "shaders/shadow_pass.geom");
	dirLightShadowPassShader.loadFromFile("shaders/directional_shadow_pass.vert", "shaders/directional_shadow_pass.frag");
	post_process_shader.loadFromFile("shaders/post_process.vert", "shaders/post_process.frag");

	geometryPassShader.loadFromFile("shaders/deferred/geometry_pass.vert", "shaders/deferred/geometry_pass.frag");
	lightingPrepassShader.loadFromFile("shaders/deferred/lighting_prepass.vert", "shaders/deferred/lighting_prepass.frag");
	lightingPassShader.loadFromFile("shaders/deferred/lighting_pass.vert", "shaders/deferred/lighting_pass.frag");

	//skybox.loadFromFiles({
	//	"assets/skybox/blue_rt.tga",
	//	"assets/skybox/blue_lf.tga",
	//	"assets/skybox/blue_up.tga",
	//	"assets/skybox/blue_dn.tga",
	//	"assets/skybox/blue_bk.tga",
	//	"assets/skybox/blue_ft.tga"
	//});

	skybox.loadFromHDRFile("assets/skybox/Ice_Lake_Env.hdr");

	postProcessTexture.allocate(gl::RGB16F, 1280, 720);
	postProcessDepthTexture.allocate(gl::DEPTH24_STENCIL8, 1280, 720);
	postProcessBuffer.attach(gl::COLOR_ATTACHMENT0, postProcessTexture);
	postProcessBuffer.attach(gl::DEPTH_STENCIL_ATTACHMENT, postProcessDepthTexture);


	geometryPositions.allocate(gl::RGB16F, 1280, 720);
	geometryNormals.allocate(gl::RGB16F, 1280, 720);
	geometryDepth.allocate(gl::DEPTH24_STENCIL8, 1280, 720);
	geometryRoughnessMetal.allocate(gl::RG16F, 1280, 720);
	geometryAlbedo.allocate(gl::RGB16F, 1280, 720);

	geometryBuffer.attach(gl::COLOR_ATTACHMENT0, geometryPositions);
	geometryBuffer.attach(gl::COLOR_ATTACHMENT1, geometryNormals);
	geometryBuffer.attach(gl::DEPTH_STENCIL_ATTACHMENT, geometryDepth);
	geometryBuffer.attach(gl::COLOR_ATTACHMENT2, geometryRoughnessMetal);
	geometryBuffer.attach(gl::COLOR_ATTACHMENT3, geometryAlbedo);

	lightingRadiance.allocate(gl::RGB16F, 1280, 720);
	lightingBrdf.allocate(gl::RGB16F, 1280, 720);
	lightingKd.allocate(gl::RGB16F, 1280, 720);

	lightingBuffer.attach(gl::COLOR_ATTACHMENT0, lightingRadiance);
	lightingBuffer.attach(gl::COLOR_ATTACHMENT1, lightingBrdf);
	lightingBuffer.attach(gl::COLOR_ATTACHMENT2, lightingKd);

	mainFramebufferTexture.allocate(gl::RGBA32F, 1280, 720);
	mainFramebuffer.attach(gl::COLOR_ATTACHMENT0, mainFramebufferTexture);
	mainFramebuffer.attach(gl::DEPTH_STENCIL_ATTACHMENT, geometryDepth);
}

void Renderer::setRenderSettings(const Shader& shader) {
	shader.setUniform("enableSSAO", settings.enableSSAO);
}

// Far and near planes for point lights.
constexpr float POINT_LIGHT_DEPTH_MAP_NEAR_PLANE = 0.1f;
constexpr float POINT_LIGHT_DEPTH_MAP_FAR_PLANE = 15.f;

constexpr float DIRECTIONAL_LIGHT_DEPTH_MAP_NEAR_PLANE = 10.f;
constexpr float DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE = 300.f;

//#define GL_WIREFRAME

void Renderer::buildSSAOTexture() {

}

void Renderer::buildShadowMaps() {

	gl::Disable(gl::BLEND);
	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);
	gl::Enable(gl::CULL_FACE);

	gl::ClearColor(0, 0, 0, 1);

	shadowPassShader.bind();
	shadowPassShader.setUniform("far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

	auto& pointLights = engine.getAllComponentsOfType<PointLight>();
	while (pointLights.size() > shadow_maps.size()) {
		shadow_maps.emplace_back(glm::vec2(1024, 1024), CUBE_DEPTH);
	}
	auto& iterator = pointLights.begin();

	for (int i = 0; i < pointLights.size(); i++, iterator++) {
		Framebuffer& fb = shadow_maps.at(i);
		PointLight& light = *static_cast<PointLight*>(iterator->get());
		
		fb.bind();
		gl::Viewport(0, 0, fb.width, fb.height);
		gl::Clear(gl::DEPTH_BUFFER_BIT);

		Shader& shader = shadowPassShader;
		glm::mat4 shadow_projection = glm::perspective(glm::radians(90.f), 1.f, POINT_LIGHT_DEPTH_MAP_NEAR_PLANE, POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadow_projection * glm::lookAt(light.gameObject.transform.position, light.gameObject.transform.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		shader.setUniformArray("shadowMatrices", shadowTransforms.data(), shadowTransforms.size());
		shader.setUniform("light_position", light.gameObject.transform.position);

		for (auto& _it : engine.getAllComponentsOfType<MeshRenderer>()) {
			auto it = static_cast<MeshRenderer*>(_it.get());

			shader.setUniform("model", it->gameObject.transform.getModelMatrix());
			shader.setUniform("material", *it->material);

			it->mesh->draw();
		}
	}
}

void Renderer::geometryPass() {
	gl::Viewport(0, 0, 1280, 720);

	geometryBuffer.bind();

	glutil::setDrawBuffers({ gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1, gl::COLOR_ATTACHMENT2, gl::COLOR_ATTACHMENT3 });
	glutil::clear({ 0, 0, 0, 1 }, gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	for (auto& it : engine.getAllComponentsOfType<MeshRenderer>()) {
		MeshRenderer* renderer = static_cast<MeshRenderer*>(it.get());

		Material& material = *renderer->material;
		Shader& shader = material.geometryPassShader;
		shader.bind();
		shader.setUniform("view", camera.getViewMatrix());
		shader.setUniform("projection", camera.projection);
		shader.setUniform("model", it->gameObject.transform.getModelMatrix());

		MaterialInstance instance = material.defaultMaterialInstance;

		for (int i = 0; i < material.samplers.size(); i++) {
			engine.getTexture(instance.samplerLocations[i])->bind(material.samplers[i].binding);
		}

		shader.setUniform("uvScale", 1.f);

		renderer->mesh->draw();
	}
}

void Renderer::render() {
	//buildShadowMaps();
	geometryPass();

	if (settings.enableSSAO) buildSSAOTexture();

	auto& pointLights = engine.getAllComponentsOfType<PointLight>();

	auto setUniforms = [&](Shader& shader) {
		shader.bind();
		shader.setUniform("camera_position", camera.transform.position);
		shader.setUniform("view", camera.getViewMatrix());
		shader.setUniform("projection", camera.projection);
		shader.setUniform("shadow_far_plane", POINT_LIGHT_DEPTH_MAP_FAR_PLANE);

		
		shader.setUniform("point_light_count", (int)pointLights.size());
		auto& iterator = pointLights.begin();
		for (int i = 0; i < pointLights.size(); i++, iterator++) {
			shader.setUniform("point_lights[" + std::to_string(i) + "]", *static_cast<PointLight*>(iterator->get()));
			shader.setUniform("shadow_map_" + std::to_string(i), 5 + i);
		//	shadow_maps.at(i).bindTexture(5 + i);
		}

		/*shader.setUniform("directional_light_count", (int)directional_lights.size());
		for (int i = 0; i < directional_lights.size(); i++) {
			shader.setUniform("directional_lights[" + std::to_string(i) + "]", directional_lights.at(i));
			shader.setUniform("dir_shadow_map_" + std::to_string(i), 21 + i);
			directional_shadow_maps.at(i).bindTexture(21 + i);

			Framebuffer& fb = directional_shadow_maps.at(i);
			DirectionalLight& light = directional_lights.at(i);

			glm::mat4 shadow_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, DIRECTIONAL_LIGHT_DEPTH_MAP_NEAR_PLANE, DIRECTIONAL_LIGHT_DEPTH_MAP_FAR_PLANE);
			glm::mat4 shadow_view = glm::lookAt(-light.direction * 100.f, { 0, 0, 0, }, { 0, 1, 0 });

			shader.setUniform("dir_light_space_matrix_" + std::to_string(i), shadow_projection * shadow_view);
		}*/
	};

	// Lighting prepass
	lightingBuffer.bind();
	gl::Clear(gl::COLOR_BUFFER_BIT);
	GLuint attachmentsPrepass[] = { gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1, gl::COLOR_ATTACHMENT2 };
	gl::DrawBuffers(ARRAYSIZE(attachmentsPrepass), attachmentsPrepass);

	// Screen sized quad
	Mesh canvas = Mesh::generatePlane({ 2, 2 });


	gl::Disable(gl::DEPTH_TEST);
	gl::Disable(gl::CULL_FACE);
	gl::Disable(gl::BLEND);


	lightingPrepassShader.bind();
	//setRenderSettings(lightingPrepassShader);
	setUniforms(lightingPrepassShader);
	lightingPrepassShader.setUniform("tex2D_geoPositions", 9);
	lightingPrepassShader.setUniform("tex2D_geoNormals", 1);
	lightingPrepassShader.setUniform("tex2D_geoRoughnessMetal", 2);
	lightingPrepassShader.setUniform("tex2D_geoAlbedo", 3);
	lightingPrepassShader.setUniform("tex2D_ambientIrradiance", 10);

	skybox.convolutedCubemap->bind(10);
	geometryPositions.bind(9);
	geometryNormals.bind(1);
	geometryRoughnessMetal.bind(2);
	geometryAlbedo.bind(3);


	canvas.draw();

	// Main lighting pass
	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LEQUAL);

	mainFramebuffer.bind();

	gl::Clear(gl::COLOR_BUFFER_BIT);

	lightingPassShader.bind();
	setUniforms(lightingPassShader);

	lightingPassShader.setUniform("tex2D_radiance", 4);
	lightingPassShader.setUniform("tex2D_brdf", 5);
	lightingPassShader.setUniform("tex2D_Kd", 6);

	lightingRadiance.bind(4);
	lightingBrdf.bind(5);
	lightingKd.bind(6);

	for (auto& it : engine.getAllComponentsOfType<MeshRenderer>()) {
		Shader& shader = lightingPassShader;
		MeshRenderer& renderer = static_cast<MeshRenderer&>(*it.get());

		shader.setUniform("model", it->gameObject.transform.getModelMatrix());
		//shader.setUniform("material", it->material);

		renderer.mesh->draw();
	}


	shadowPassShader.bind();
	skybox.render(camera.getViewMatrix(), camera.projection);

	// Transparency
	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::BACK);
	gl::FrontFace(gl::CCW);

	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	/*std::sort(transparents.begin(), transparents.end(), [&](const Model* a, const Model* b) {
		return glm::distance(camera.transform.position, a->transform.position) > glm::distance(camera.transform.position, b->transform.position);
	});


	for (auto& it : transparents) {
		Shader& shader = shadowPassShader;
		int i = 0;
		for (auto it2 = it->material.textures.begin(); it2 != it->material.textures.end(); it2++) {
			shader.setUniform(it2->first, i);
			it2->second->bind(i);

			i++;
		}

		shader.setUniform("model", it->transform.getModelMatrix());
		shader.setUniform("material", it->material);

		it->mesh->draw();
	}
	*/

	// Post processing
	gl::Disable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);


	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT);

	post_process_shader.bind();
	post_process_shader.setUniform("screen_capture", 0);
	mainFramebufferTexture.bind(0);
	//geometryAlbedo.bind(0);


	canvas.draw();
}

