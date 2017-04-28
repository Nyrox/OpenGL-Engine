#pragma once
#include <2D\Font.h>
#include <Core\Shader.h>

class Text {
public:
	explicit Text(Font* font, std::string string = std::string());

	void setString(const std::string& string);
	void setFont(Font* font);
	void render();

	glm::vec2 position;
	glm::vec3 fillColor;
private:
	Shader shader;
	Font* font = nullptr;

	std::string buffer;

	// most temporary of solutions
	GLuint vao, vbo;
};