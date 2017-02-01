#include "Debug.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <algorithm>

struct Line {
	Line(glm::vec3 t_start, glm::vec3 t_end, float t_duration) : start(t_start), end(t_end) {
		begin_time = glfwGetTime();
		end_time = begin_time + t_duration;

		generateVao();
	}
	glm::vec3 start;
	glm::vec3 end;
	double end_time;
	double begin_time;

	GLuint vao;

private:
	void generateVao() {
		GLfloat vertices[] = {
			0, 0, 0,
			end.x, end.y, end.z
		};

		GLuint vbo;
		gl::GenVertexArrays(1, &vao);
		gl::GenBuffers(1, &vbo);

		gl::BindVertexArray(vao);
		gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
		gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

		gl::VertexAttribPointer(0, 3, gl::FLOAT, 0, 3 * sizeof(GLfloat), (GLvoid*)0);
		gl::EnableVertexAttribArray(0);
	};
};

Debug::Debug() {
	line_shader.loadFromFile("shaders/debug/line.vert", "shaders/debug/line.frag");
}

// STATIC
Debug& Debug::getInstance() {
	static Debug instance;
	return instance;
}

// STATIC
void Debug::drawLine(glm::vec3 start, glm::vec3 end, float duration) {
	Debug& _this = Debug::getInstance();

	_this.lines.emplace_back(start, end, duration);
}

// STATIC
void Debug::render(glm::mat4 view, glm::mat4 projection) {
	Debug& _this = Debug::getInstance();
	for (auto &it : _this.lines) {
		_this.internal_renderLine(it, view, projection);
	}

	// Find an iterator to the new logical end of the container
	auto end_iterator = std::remove_if(_this.lines.begin(), _this.lines.end(), [](const Line& line) {
		return glfwGetTime() >= line.end_time;
	});
	// Reduce the container to have its physical size match its new logical size
	_this.lines.erase(end_iterator, _this.lines.end());

}

void Debug::internal_renderLine(const Line& line, glm::mat4 view, glm::mat4 projection) {
	// Translate the ray to it's start position
	glm::mat4 model = glm::translate(line.start);

	line_shader.bind();
	line_shader.setUniform("model", model);
	line_shader.setUniform("view", view);
	line_shader.setUniform("projection", projection);

	
	gl::BindVertexArray(line.vao);
	gl::LineWidth(2);
	gl::DrawArrays(gl::LINES, 0, 2);
	gl::BindVertexArray(0);

	gl::UseProgram(0);
}