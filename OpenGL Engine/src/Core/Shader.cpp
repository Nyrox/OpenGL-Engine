#include "Shader.h"
#include <Core\NShaderCompiler\ShaderCompiler.h>

#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Shader& Shader::bind() {
	gl::UseProgram(this->id);
	return *this;
}

void Shader::loadFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& gShaderFile) {
	compile(loadSingle(vShaderFile), loadSingle(fShaderFile), (!gShaderFile.empty() ? loadSingle(gShaderFile) : std::string()));
}

void Shader::compile(const std::string& t_vertexSource, const std::string& t_fragmentSource, const std::string& t_geometrySource) {
	GLuint vertex, fragment, geometry;

	std::string tempv = compileShader(t_vertexSource, { "./shaders" }).c_str();
	std::string tempf = compileShader(t_fragmentSource, { "./shaders" }).c_str();
	std::string tempg = (t_geometrySource.empty() ? std::string() : compileShader(t_geometrySource, { "./shaders" }).c_str());
	const char* vertexSource = tempv.c_str();
	const char* fragmentSource = tempf.c_str();
	const char* geometrySource = (tempg.empty() ? nullptr : tempg.c_str());

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

void Shader::setUniform(std::string name, int val) {
	gl::Uniform1i(gl::GetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, float val) {
	gl::Uniform1f(gl::GetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, glm::vec2 val) {
	gl::Uniform2fv(gl::GetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::vec3 val) {
	gl::Uniform3fv(gl::GetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::mat4 matrix) {
	gl::UniformMatrix4fv(gl::GetUniformLocation(this->id, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void Shader::setUniform(std::string name, Material material) {
	setUniform(name + ".diffuse", 0);
	setUniform(name + ".specular", 1);
	setUniform(name + ".shininess", material.shininess);
}

void Shader::setUniform(std::string name, PointLight light, uint32_t index) {
	setUniform(name + ".position", light.position);
	setUniform(name + ".ambient", light.color);
	setUniform(name + ".diffuse", light.color);
	setUniform(name + ".specular", light.color);
	setUniform(name + ".constant", light.constant);
	setUniform(name + ".linear", light.linear);
	setUniform(name + ".quadratic", light.quadratic);
}

void Shader::setUniform(std::string name, DirectionalLight light, uint32_t index) {
	setUniform(name + ".direction", light.direction);
	setUniform(name + ".ambient", light.ambient);
	setUniform(name + ".diffuse", light.diffuse);
	setUniform(name + ".specular", light.specular);
}


void Shader::setUniformArray(std::string name, glm::mat4* data, uint32_t count) {
	gl::UniformMatrix4fv(gl::GetUniformLocation(this->id, name.c_str()), count, 0, (GLfloat*)data);
}

void Shader::setUniformArray(std::string name, PointLight* data, uint32_t count) {
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