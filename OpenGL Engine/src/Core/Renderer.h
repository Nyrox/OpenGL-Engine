#pragma once
#include <Core/Framebuffer.h>
#include <Core/Camera.h>
#include <Core/Shader.h>
#include <Core/Model.h>
#include <Core/Skybox.h>
#include <Core/Texture2D.h>
#include <vector>

class Engine;

class Renderer {
public:
	Renderer(Engine& engine, Camera& camera, float backbuffer_width, float backbuffer_height);
	
	void render();
	void setRenderSettings(const Shader& shader);

	struct RenderSettings {
		bool enableSSAO = true;
	} settings;

	Camera& camera;
private:
	Engine& engine;
	
	void buildSSAOTexture();
	void buildShadowMaps();
	void geometryPass();
	void renderPass();

	Skybox skybox;
	Refactor::Framebuffer postProcessBuffer;
	Texture2D postProcessTexture;
	Texture2D postProcessDepthTexture;

	Shader post_process_shader;
	Shader shadowPassShader;
	Shader dirLightShadowPassShader;

	std::vector<Framebuffer> shadow_maps;

	Shader geometryPassShader;
	Refactor::Framebuffer geometryBuffer;
	Texture2D geometryPositions;
	Texture2D geometryNormals;
	Texture2D geometryDepth;
	Texture2D geometryRoughnessMetal;
	Texture2D geometryAlbedo;

	Shader lightingPrepassShader;
	Refactor::Framebuffer lightingBuffer;
	Texture2D lightingRadiance;
	Texture2D lightingBrdf;
	Texture2D lightingKd;

	Shader lightingPassShader;
	Refactor::Framebuffer mainFramebuffer;
	Texture2D mainFramebufferTexture;
};