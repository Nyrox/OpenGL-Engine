#pragma once
#include <Core/Light.h>

class Material;

/*
	Manages a shader program
	TODO: Implement some basic preprocessing
*/
class Shader {
public:
	Shader() = default;
	Shader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile = std::string());

	GLuint id;
	void bind() const;

	/*
		Loads shader source from files and compiles the program
		Basically a wrapper around compile.
		@[params]
		@vShaderFile: File location of the vertex shader source.
		@fShaderFile: File location of the fragment shader source.
		@gShaderFile: File location of the geometry shader source.
					  If none is provided the program will be compiled without a geometry shader.
	*/
	void loadFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile = std::string());

	/*
		Given shader source strings compiles them to a program
		@[params]
		@vertexSource: Vertex shader source.
		@fragmentSource: Fragment shader source.
		@geometrySource: Geometry shader source.
						 If none is provided the program will be compiled without a geometry shader.
	*/
	void compile(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = nullptr);

	void setUniform(std::string name, int val) const;
	void setUniform(std::string name, float val) const;
	void setUniform(std::string name, glm::vec2) const;
	void setUniform(std::string name, glm::vec3) const;
	void setUniform(std::string name, glm::mat4 matrix) const;

	void setUniform(std::string name, const Material& material) const;
	void setUniform(std::string name, const PointLight& light, uint32_t index = 0) const;
	void setUniform(std::string name, const DirectionalLight& light, uint32_t index = 0) const;


	void setUniformArray(std::string name, glm::mat4* data, uint32_t count) const;
	void setUniformArray(std::string name, PointLight* data, uint32_t count) const;
	
private:
	/*
		Helper function for checking compile errors and giving back useful error messages
	*/
	void checkCompileErrors(GLuint object, GLenum type);

	/*
		Helper function for lor loading individual shaders.
	*/
	std::string loadSingle(const std::string& shaderFile);

	
};