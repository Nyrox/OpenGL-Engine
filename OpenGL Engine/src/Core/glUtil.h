#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gl_core_4_3.hpp>

/*
	A collection of helpful methods to make opengl calls more compact
*/

namespace glutil {
	// Wraps gl::ClearColor and gl::Clear into one call
	// Has a small driver overhead obviously when clearing the same color multiple times
	void clear(glm::vec4 color, GLenum bits) {
		gl::ClearColor(color.r, color.b, color.g, color.z);
		gl::Clear(bits);
	}

	// Essentially a wrapper around gl::DrawBuffers
	void setDrawBuffers(std::initializer_list<GLenum> attachments) {
		gl::DrawBuffers(attachments.size(), attachments.begin());
	}
}
