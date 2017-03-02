#include "Text.h"
#include <glm\gtx\transform.hpp>

Text::Text() {
	shader.loadFromFile("shaders/text.vert", "shaders/text.frag");


	gl::CreateVertexArrays(1, &vao);
	gl::CreateBuffers(1, &vbo);

	gl::BindVertexArray(vao);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::NamedBufferData(vbo, sizeof(GLfloat) * 6 * 4, NULL, gl::DYNAMIC_DRAW);
	
	gl::VertexAttribPointer(0, 2, gl::FLOAT, false, 4 * sizeof(GLfloat), 0);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, false, 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 2));
	gl::EnableVertexAttribArray(1);

	gl::BindVertexArray(0);
}

void Text::setFont(Font* t_font) {
	font = t_font;
}

void Text::setString(const std::string& string) {
	buffer = string;
}

void Text::render() {
	glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.0f, -100.f, 100.f);

	gl::Disable(gl::CULL_FACE);
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	shader.bind();
	shader.setUniform("projection", projection);
	shader.setUniform("fillColor", glm::vec3(1.0, 0.0, 0.0));

	gl::BindVertexArray(vao);

	float xPosition = 350;
	float yPosition = 50;

	for (auto& it : buffer) {
		Glyph& character = font->getGlyph(it);
		gl::ActiveTexture(gl::TEXTURE0);
		gl::BindTexture(gl::TEXTURE_2D, character.textureID);


		float xFinal = xPosition + character.bearing.x;
		float yFinal = yPosition - (float)(character.size.y - character.bearing.y);
		float width = character.size.x;
		float height = character.size.y;

		GLfloat vertices[6][4] = {
			{ xFinal, yFinal - height,			0.0, 0.0 },
			{ xFinal, yFinal,					0.0, 1.0 },
			{ xFinal + width, yFinal,			1.0, 1.0 },

			{ xFinal, yFinal - height,			0.0, 0.0 },
			{ xFinal + width, yFinal,			1.0, 1.0 },
			{ xFinal + width, yFinal - height,	1.0, 0.0 }
		};
		
		gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
		gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		gl::DrawArrays(gl::TRIANGLES, 0, 6);

		xPosition += (character.advance >> 6);
	}
	
	gl::BindVertexArray(0);
}