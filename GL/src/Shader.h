#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
public:
	GLuint id;
	Shader& bind();

	void loadFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	void setUniform(std::string name, float val);
	void setUniform(std::string name, glm::vec2);
	void setUniform(std::string name, glm::vec3);
	void setUniform(std::string name, glm::mat4 matrix);

private:
	void checkCompileErrors(GLuint object, GLenum type);
	std::string loadSingle(const char* shaderFile);

	
};