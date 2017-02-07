#pragma once
#include <gl_core_4_3.hpp>
#include <glm\glm.hpp>
#include <string>


/*
	Responsible for loading 2D textures.
	TODO: Rethink the purpose and implementation of this class.
*/
class Texture {
public:
	Texture();

	GLuint id;
	int32_t width, height;

	void loadFromFile(std::string filename, GLenum format);
	void bind(int tu);
};

namespace Refactor {
	
	class Texture2D {
	public:
		Texture2D(bool mipmaps = true, GLenum texture_wrap = gl::REPEAT, GLenum filtering = gl::LINEAR);
		~Texture2D();

		void bind(uint32_t texture_unit);
		void allocate(GLenum format, glm::vec2 size);


		bool mipmaps;
		GLenum texture_wrap;
		GLenum filtering;

		GLuint handle;
		int32_t width, height;
	};

}