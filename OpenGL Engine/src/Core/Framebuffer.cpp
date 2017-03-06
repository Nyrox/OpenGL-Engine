#include "Framebuffer.h"
#include <Core/Texture2D.h>
#include <Core/Texture3D.h>

#include <iostream>

namespace Refactor {
	
	Framebuffer::Framebuffer() {
		gl::CreateFramebuffers(1, &handle);
	}
	
	Framebuffer::~Framebuffer() {
		gl::DeleteFramebuffers(1, &handle);
	}


	void Framebuffer::bind() {
		gl::BindFramebuffer(gl::FRAMEBUFFER, handle);
	}

	void Framebuffer::attach(GLenum attach, const Texture2D& texture) {
		bind();
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, attach, gl::TEXTURE_2D, texture.handle, 0);
	}

	void Framebuffer::attach(GLenum attach, GLenum target, const Texture3D& texture) {
		bind();
		for (uint8_t i = 0; i < 6; i++) {
			gl::FramebufferTexture2D(gl::FRAMEBUFFER, attach + i, target, texture.handle, 0);
		}
	}


}


Framebuffer::~Framebuffer() {
	if(fbo != 0)
		gl::DeleteFramebuffers(1, &fbo);
}

Framebuffer::Framebuffer(Framebuffer&& other) {
	fbo = other.fbo;
	tex = other.tex;
	width = other.width;
	height = other.height;
	type = other.type;

	other.fbo = 0;
}

Framebuffer::Framebuffer(glm::vec2 size, FramebufferTypes type, bool depthBuffer) : width(size.x), height(size.y), type(type) {
	gl::GenFramebuffers(1, &fbo);
	this->bind();

	if (type == DEPTH || type == CUBE_DEPTH) depthBuffer = false;

	if (depthBuffer) {
		GLuint rbo;
		gl::GenRenderbuffers(1, &rbo);
		gl::BindRenderbuffer(gl::RENDERBUFFER, rbo);
		gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH24_STENCIL8, size.x, size.y);
		gl::BindRenderbuffer(gl::RENDERBUFFER, 0);
		gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_STENCIL_ATTACHMENT, gl::RENDERBUFFER, rbo);
	}

	GLenum format = type == RGB ? gl::RGB : gl::DEPTH_COMPONENT;
	GLenum attach = type == RGB ? gl::COLOR_ATTACHMENT0 : gl::DEPTH_ATTACHMENT;

	gl::GenTextures(1, &tex);
	this->bindTexture();

	if (type == RGB) {
		gl::TexImage2D(gl::TEXTURE_2D, 0, format, size.x, size.y, 0, format, gl::FLOAT, NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
	}
		
	if (type == DEPTH) {
		gl::TexImage2D(gl::TEXTURE_2D, 0, format, size.x, size.y, 0, format, gl::FLOAT, NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, borderColor);
	}

	if (type == CUBE_DEPTH || type == CUBE_RGB) {
		for (int32_t i = 0; i < 6; i++) {
			gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, gl::FLOAT, NULL);
		}

		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::NEAREST);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::NEAREST);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);

		gl::FramebufferTexture(gl::FRAMEBUFFER, attach, tex, 0);
	}
	else {
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, attach, gl::TEXTURE_2D, tex, 0);
	}
	
	if (type == DEPTH || type == CUBE_DEPTH) {
		gl::DrawBuffer(gl::NONE);
		gl::ReadBuffer(gl::NONE);
	}

	if (gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Initializing framebuffer failed." << std::endl;
	}
}

void Framebuffer::bind() {
	gl::BindFramebuffer(gl::FRAMEBUFFER, fbo);
}

void Framebuffer::bindTexture(uint32_t index) {
	gl::ActiveTexture(gl::TEXTURE0 + index);

	gl::BindTexture((type == CUBE_RGB || type == CUBE_DEPTH) ? gl::TEXTURE_CUBE_MAP : gl::TEXTURE_2D, tex);
}
