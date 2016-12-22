#pragma once
#include <GL/glew.h>

#include <string>

#include <Material.h>
#include <Light.h>

class Shader {
public:
	GLuint id;
	Shader& bind();

	void loadFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	void setUniform(std::string name, int val);
	void setUniform(std::string name, float val);
	void setUniform(std::string name, glm::vec2);
	void setUniform(std::string name, glm::vec3);
	void setUniform(std::string name, glm::mat4 matrix);

	void setUniform(std::string name, Material material);
	void setUniform(std::string name, PointLight light, uint32_t index = 0);
private:
	void checkCompileErrors(GLuint object, GLenum type);
	std::string loadSingle(const char* shaderFile);

	
};