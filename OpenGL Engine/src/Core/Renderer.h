#pragma once
#include <Core/Framebuffer.h>
#include <Core/Camera.h>
#include <Core/Shader.h>
#include <Core/Model.h>
#include <Core/Light.h>
#include <Core/Skybox.h>
#include <Core/Texture.h>
#include <list>

class Renderer {
public:
	Renderer(float backbuffer_width, float backbuffer_height);
	
	void render();

	void addPointLight(PointLight light);
	void addDirectionalLight(DirectionalLight light);

	std::list<Model> models;
	std::list<Model> transparents;

	Camera* camera;
	glm::mat4 projection;
private:
	Skybox skybox;
	Framebuffer postProcessBuffer;
	//Refactor::Texture2D postProcessTexture;

	Shader post_process_shader;
	Shader shadow_pass_shader;
	Shader dirLightShadowPassShader;

	Shader forward_render_shader;	

	std::vector<PointLight> point_lights;
	std::vector<Framebuffer> shadow_maps;
	
	std::vector<DirectionalLight> directional_lights;
	std::vector<Framebuffer> directional_shadow_maps;

};