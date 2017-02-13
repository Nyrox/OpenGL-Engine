#pragma once
#include <2D\Font.h>
#include <Core\Shader.h>

class Text {
public:
	Text();

	void setString(const std::string& string);
	void setFont(Font* font);
	void render();

private:
	Shader shader;
	Font* font = nullptr;

	std::string buffer;

	// most temporary of solutions
	GLuint vao, vbo;
};