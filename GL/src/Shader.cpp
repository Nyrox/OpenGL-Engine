#include "Shader.h"

#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Shader& Shader::bind() {
	glUseProgram(this->id);
	return *this;
}

void Shader::loadFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
	compile(loadSingle(vShaderFile).c_str(), loadSingle(fShaderFile).c_str(), gShaderFile ? loadSingle(gShaderFile).c_str() : nullptr);
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
	GLuint vertex, fragment, geometry;

	/* Vertex */
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, GL_VERTEX_SHADER);

	/* Fragment */
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, GL_FRAGMENT_SHADER);

	/* Geometry */
	if (geometrySource != nullptr) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, GL_GEOMETRY_SHADER);
	}

	this->id = glCreateProgram();
	glAttachShader(this->id, vertex);
	glAttachShader(this->id, fragment);
	geometrySource ? glAttachShader(this->id, geometry) : 0;

	glLinkProgram(this->id);
	checkCompileErrors(this->id, GL_PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	geometrySource ? glDeleteShader(geometry) : 0;
}

std::string Shader::loadSingle(const char* shaderFile) {
	std::stringstream out;
	try {
		std::ifstream file(shaderFile);
		out << file.rdbuf();
		file.close();
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed loading file: \n" << e.what() << std::endl;
	}

	return out.str();
}

void Shader::checkCompileErrors(GLuint object, GLenum type) {
	GLint success;
	GLchar infoLog[1024];

	if (type == GL_PROGRAM) {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
		}
	}
	else {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
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
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, float val) {
	glUniform1f(glGetUniformLocation(this->id, name.c_str()), val);
}

void Shader::setUniform(std::string name, glm::vec2 val) {
	glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::vec3 val) {
	glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(val));
}

void Shader::setUniform(std::string name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void Shader::setUniform(std::string name, Material material) {
	setUniform(name + ".diffuse", 0);
	setUniform(name + ".specular", 1);
	setUniform(name + ".shininess", material.shininess);
}

void Shader::setUniform(std::string name, PointLight light, uint32_t index) {
	setUniform(name + "[" + std::to_string(index) + "]" + ".position", light.position);
	setUniform(name + "[" + std::to_string(index) + "]" + ".ambient", light.ambient);
	setUniform(name + "[" + std::to_string(index) + "]" + ".diffuse", light.diffuse);
	setUniform(name + "[" + std::to_string(index) + "]" + ".specular", light.specular);
	setUniform(name + "[" + std::to_string(index) + "]" + ".constant", light.constant);
	setUniform(name + "[" + std::to_string(index) + "]" + ".linear", light.linear);
	setUniform(name + "[" + std::to_string(index) + "]" + ".quadratic", light.quadratic);
}