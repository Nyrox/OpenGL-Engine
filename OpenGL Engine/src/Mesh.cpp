#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

bool str_starts_with(std::string haystack, std::string needle) {
	// If the needle is bigger than the haystack, it can't possibly be contained in the haystack.
	// While a small overhead for small strings, this can yield massive savings working with chaotic strings
	// As a neat plus it also prevents us from accidentally running out of the bounds of our haystack when comparing
	if (needle.size() > haystack.size()) return false;

	for (int i = 0; i < needle.size(); i++) {
		if (haystack[i] != needle[i]) return false;
	}

	return true;
}

std::vector<std::string> split(const std::string& haystack, char delim) {
	std::vector<std::string> tokens;
	
	uint32_t prev = 0, pos = 0;
	do {
		pos = haystack.find(delim, prev);
		if (pos == std::string::npos) pos = haystack.length();
		std::string token = haystack.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + 1;
	} while (pos < haystack.length() && prev < haystack.length());
	
	return tokens;
}

Mesh::Mesh() {
	scale = glm::vec3(1);
}

void Mesh::draw(Shader& shader) {
	shader.bind();
	shader.setUniform("model", glm::scale(glm::translate(glm::mat4(), position), scale));
	shader.setUniform("material", material);

	gl::BindVertexArray(this->vao);
	gl::DrawElements(gl::TRIANGLES, indices.size(), gl::UNSIGNED_INT, 0);
	gl::BindVertexArray(0);
}

void Mesh::loadFromFile(std::string path) {
	std::stringstream ss;
	ss << std::ifstream(path).rdbuf();
	
	// Needs comment
	bool reachedData = false;

	// Contains the current line
	std::string to;

	// Parse header
	while (!reachedData && std::getline(ss, to)) {
		auto tokens = split(to, ' ');		
		
		if (tokens[0] == "element") {
			if		(tokens[1] == "vertex") vertexCount = std::stof(tokens[2]);
			else if (tokens[1] == "face") faceCount = std::stof(tokens[2]);
			else {
				std::cout << "Unrecognized element in .ply import :" << tokens[1] << std::endl;
			}
			continue;
		}

		if (tokens[0] == "end_header") {
			reachedData = true;
			continue;
		}
	}


	// Store which vertex we are currently on
	uint32_t vi = 0;

	// Parse vertices
	while (vi < vertexCount && std::getline(ss, to)) {
		auto tokens = split(to, ' ');
		std::vector<float> values;
		for (int i = 0; i < tokens.size(); i++) {
			values.push_back(std::stof(tokens[i]));
		}

		vertices.push_back(Vertex(glm::vec3(values[0], values[1], values[2]), glm::vec3(values[3], values[4], values[5]), glm::vec2(values[6], values[7])));

		vi++;
	}

	// Store which face we are currently on
	uint32_t fi = 0;

	// Parse faces
	while (std::getline(ss, to)) {
		auto tokens = split(to, ' ');
		std::vector<int> values;
		for (int i = 0; i < tokens.size(); i++) {
			values.push_back(std::stoi(tokens[i]));
		}

		// Check how many vertices are contained in our face
		switch (values[0]) {
		case 3:

			break;
		case 4:
			// TODO: Benchmark how much this reserve helps, if at all
			indices.reserve(indices.size() + 6);

			indices.push_back(values[1]);
			indices.push_back(values[2]);
			indices.push_back(values[3]);
			indices.push_back(values[1]);
			indices.push_back(values[3]);
			indices.push_back(values[4]);
			break;
		}
	
		fi++;
	}

	initRenderData();
}

void Mesh::loadFromMemory(std::vector<Vertex> t_vertices, std::vector<uint32_t> t_indices) {
	vertices = t_vertices;
	indices = t_indices;

	initRenderData();
}

void Mesh::initRenderData() {
	GLuint vbo, ebo;

	gl::GenVertexArrays(1, &this->vao);
	gl::GenBuffers(1, &vbo);
	gl::GenBuffers(1, &ebo);

	gl::BindVertexArray(this->vao);

	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), gl::STATIC_DRAW);

	
	// Positions
	gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);
	// Normals
	gl::VertexAttribPointer(1, 3, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);
	// Texture Coords
	gl::VertexAttribPointer(2, 2, gl::FLOAT, 0, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);

	gl::BindVertexArray(0);
}