#pragma once
#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>
#include <string>


class Texture2D {
public:
	Texture2D(bool mipmaps = true, GLenum texture_wrap = gl::REPEAT, GLenum filtering = gl::LINEAR);
	Texture2D(Texture2D&& other);

	~Texture2D();

	void bind(uint32_t texture_unit);
	void allocate(GLenum format, glm::vec2 size);

	/*
		Loads a texture from file
		Calling this without calling allocate first will result in a crash
	*/
	void loadFromFile(const std::string& file, GLenum pixelFormat = 0);

	bool mipmaps;
	GLenum texture_wrap;
	GLenum filtering;
	GLenum internalFormat;

	GLuint handle;
	int32_t width, height;

private:
	Texture2D(const Texture2D& other) = delete;

};


