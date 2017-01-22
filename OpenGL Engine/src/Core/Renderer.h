#pragma once
#include <Framebuffer.h>
#include <Core/Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <Light.h>

#include <list>
#include <vector>

class Renderer {
public:
	Renderer(float backbuffer_width, float backbuffer_height);
	
	void render();

	void addPointLight(PointLight light);

	std::list<Mesh> meshes;
	Camera* camera;
	glm::mat4 projection;
private:
	Framebuffer postProcessBuffer;

	Shader post_process_shader;
	Shader shadow_pass_shader;
	Shader forward_render_shader;	

	std::vector<PointLight> point_lights;
	std::vector<Framebuffer> shadow_maps;
		
};