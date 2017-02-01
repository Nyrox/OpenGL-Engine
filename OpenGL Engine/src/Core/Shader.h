#pragma once
#include <Core/Material.h>
#include <Core/Light.h>


/*
	Manages a shader program
	TODO: Implement some basic preprocessing
*/
class Shader {
public:
	GLuint id;
	Shader& bind();

	/*
		Loads shader source from files and compiles the program
		Basically a wrapper around compile.
		@[params]
		@vShaderFile: File location of the vertex shader source.
		@fShaderFile: File location of the fragment shader source.
		@gShaderFile: File location of the geometry shader source.
					  If none is provided the program will be compiled without a geometry shader.
	*/
	void loadFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	/*
		Given shader source strings compiles them to a program
		@[params]
		@vertexSource: Vertex shader source.
		@fragmentSource: Fragment shader source.
		@geometrySource: Geometry shader source.
						 If none is provided the program will be compiled without a geometry shader.
	*/
	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

	
	void setUniform(std::string name, int val);
	void setUniform(std::string name, float val);
	void setUniform(std::string name, glm::vec2);
	void setUniform(std::string name, glm::vec3);
	void setUniform(std::string name, glm::mat4 matrix);

	void setUniform(std::string name, Material material);
	void setUniform(std::string name, PointLight light, uint32_t index = 0);
	void setUniform(std::string name, DirectionalLight light, uint32_t index = 0);


	void setUniformArray(std::string name, glm::mat4* data, uint32_t count);
	void setUniformArray(std::string name, PointLight* data, uint32_t count);
	
private:
	/*
		Helper function for checking compile errors and giving back useful error messages
	*/
	void checkCompileErrors(GLuint object, GLenum type);

	/*
		Helper function for lor loading individual shaders.
	*/
	std::string loadSingle(const char* shaderFile);

	
};