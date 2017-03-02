#pragma once
#include <Core/Framebuffer.h>
#include <Core/Camera.h>
#include <Core/Shader.h>
#include <Core/Model.h>
#include <Core/Light.h>
#include <Core/Skybox.h>
#include <Core/Texture.h>
#include <vector>

class Renderer {
public:
	Renderer(Camera& camera, float backbuffer_width, float backbuffer_height);
	
	void render();
	void render_new();

	void addPointLight(PointLight light);
	void addDirectionalLight(DirectionalLight light);

	
	void insert(Model* node);
	std::vector<PointLight> point_lights;

	Camera& camera;
private:
	// Cached containers
	std::vector<Model*> opagues;
	std::vector<Model*> transparents;


	Skybox skybox;
	Refactor::Framebuffer postProcessBuffer;
	Texture2D postProcessTexture;
	Texture2D postProcessDepthTexture;

	Shader post_process_shader;
	Shader shadow_pass_shader;
	Shader dirLightShadowPassShader;

	Shader forward_render_shader;	

	
	std::vector<Framebuffer> shadow_maps;
	
	std::vector<DirectionalLight> directional_lights;
	std::vector<Framebuffer> directional_shadow_maps;


	Shader geometryPassShader;
	Refactor::Framebuffer geometryBuffer;
	Texture2D geometryPositions;
	Texture2D geometryNormals;
	Texture2D geometryDepth;
	
	Shader lightingPrepassShader;
	Refactor::Framebuffer lightingBuffer;
	Texture2D lightingAlbedoSpec;

	Shader lightingPassShader;
	Refactor::Framebuffer mainFramebuffer;
	Texture2D mainFramebufferTexture;
};