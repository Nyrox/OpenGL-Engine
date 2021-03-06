#include "Shader.h"
#include <Core\NShaderCompiler\ShaderCompiler.h>
#include <experimental\filesystem>
#include <Core/ECS/Components/PointLight.h>
#include <Core/ECS/GameObject.h>

#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <Core/Util/FileUtil.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile) {
	loadFromFile(vShaderFile, fShaderFile, gShaderFile);
}

void Shader::bind() const {
	gl::UseProgram(this->id);
}

void Shader::loadFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile) {
	ShaderCompiler compiler;
	compiler.addIncludeDirectory("shaders/");
	compiler.defineMacro("__MATERIAL__", "");
	compiler.setVertexShaderSource(FUtil::stringstream_read_file(vShaderFile).str());
	compiler.setFragmentShaderSource(FUtil::stringstream_read_file(fShaderFile).str());

	*this = compiler.compile();
}

void Shader::compile(const std::string& t_vertexSource, const std::string& t_fragmentSource, const std::string& t_geometrySource) {
	GLuint vertex, fragment, geometry;

	const char* vertexSource = t_vertexSource.c_str();
	const char* fragmentSource = t_fragmentSource.c_str();
	const char* geometrySource = (t_geometrySource.empty() ? nullptr : t_geometrySource.c_str());

	/* Vertex */
	vertex = gl::CreateShader(gl::VERTEX_SHADER);
	gl::ShaderSource(vertex, 1, &vertexSource, NULL);
	gl::CompileShader(vertex);
	checkCompileErrors(vertex, gl::VERTEX_SHADER);

	/* Fragment */
	fragment = gl::CreateShader(gl::FRAGMENT_SHADER);
	gl::ShaderSource(fragment, 1, &fragmentSource, NULL);
	gl::CompileShader(fragment);
	checkCompileErrors(fragment, gl::FRAGMENT_SHADER);

	/* Geometry */
	if (geometrySource != nullptr) {
		geometry = gl::CreateShader(gl::GEOMETRY_SHADER);
		gl::ShaderSource(geometry, 1, &geometrySource, NULL);
		gl::CompileShader(geometry);
		checkCompileErrors(geometry, gl::GEOMETRY_SHADER);
	}

	this->id = gl::CreateProgram();
	gl::AttachShader(this->id, vertex);
	gl::AttachShader(this->id, fragment);
	geometrySource ? gl::AttachShader(this->id, geometry) : 0;

	gl::LinkProgram(this->id);
	checkCompileErrors(this->id, gl::PROGRAM);

	gl::DeleteShader(vertex);
	gl::DeleteShader(fragment);
	geometrySource ? gl::DeleteShader(geometry) : 0;
}


std::string Shader::loadSingle(const std::string& shaderFile) {
	using namespace std::experimental;

	filesystem::path path(shaderFile);
	if (!filesystem::exists(path)) {
		std::cout << "Error trying to load: " << shaderFile << ". File does not exist.\n";
		return std::string();
	}	

	std::stringstream out;
	try {
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(shaderFile);

		out << file.rdbuf();
		file.close();
	}
	catch (std::exception e) {
		std::cout << "Error: Compiling shader failed. Abort ship." << std::endl;
		std::cout << "Info: Shader Source: \n" + out.str() << std::endl;
	}

	
	return out.str();
}

void Shader::checkCompileErrors(GLuint object, GLenum type) {
	GLint success;
	GLchar infoLog[1024];

	if (type == gl::PROGRAM) {
		gl::GetProgramiv(object, gl::LINK_STATUS, &success);
		if (!success) {
			gl::GetProgramInfoLog(object, 1024, NULL, infoLog);
		}
	}
	else {
		gl::GetShaderiv(object, gl::COMPILE_STATUS, &success);
		if (!success) {
			gl::GetShaderInfoLog(object, 1024, NULL, infoLog);
		}
	}
	if (!success) {
		std::cout << "ERROR::SHADER: \n" << infoLog << std::endl;

	}
}


/*
	Uniforms
*/

void Shader::setUniform(std::string name, int val) const {
	gl::Uniform1i(gl::GetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, float val) const {
	gl::Uniform1f(gl::GetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, glm::vec2 val) const {
	gl::Uniform2fv(gl::GetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::vec3 val) const {
	gl::Uniform3fv(gl::GetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::mat4 matrix) const {
	gl::UniformMatrix4fv(gl::GetUniformLocation(this->id, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void Shader::setUniform(std::string name, const Material& material) const {

}

void Shader::setUniform(std::string name, const PointLight& light, uint32_t index) const {
	setUniform(name + ".position", light.gameObject.transform.position);
	setUniform(name + ".color", light.color);
	setUniform(name + ".intensity", light.intensity);
	setUniform(name + ".constant", light.constant);
	setUniform(name + ".linear", light.linear);
	setUniform(name + ".quadratic", light.quadratic);
}

void Shader::setUniformArray(std::string name, glm::mat4* data, uint32_t count) const {
	gl::UniformMatrix4fv(gl::GetUniformLocation(this->id, name.c_str()), count, 0, (GLfloat*)data);
}

void Shader::setUniformArray(std::string name, PointLight* data, uint32_t count) const {
	/*
	setUniform(name + "[" + "0" + "]" + ".position", light.position);
	setUniform(name + "[" + "0" + "]" + ".ambient", light.ambient);
	setUniform(name + "[" + "0" + "]" + ".diffuse", light.diffuse);
	setUniform(name + "[" + "0" + "]" + ".specular", light.specular);
	setUniform(name + "[" + "0" + "]" + ".constant", light.constant);
	setUniform(name + "[" + "0" + "]" + ".linear", light.linear);
	setUniform(name + "[" + "0" + "]" + ".quadratic", light.quadratic);

	*/
}