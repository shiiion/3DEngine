#pragma once

#include <iostream>

#include "ScreenBuffer.h"

namespace ginkgo {

	ScreenBuffer::ScreenBuffer(unsigned int width, unsigned int height, vec4 clear_color, bool depth, bool stencil)
		: clear_color(clear_color)
	{
		addVertexShader("shaders/screenVertex.vs");
		addFragmentShader("shaders/screenFragment.fs");
		compileShader();

		GLfloat quadVertices[] = {
			//Positions		//Texture Coordinates
			-1.0f,  1.0f,	0.0f, 1.0f,
			-1.0f, -1.0f,	0.0f, 0.0f,
			1.0f, -1.0f,	1.0f, 0.0f,

			-1.0f,  1.0f,	0.0f, 1.0f,
			1.0f, -1.0f,	1.0f, 0.0f,
			1.0f,  1.0f,	1.0f, 1.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		GLenum attachment_type;
		if (!depth && !stencil)
			attachment_type = GL_RGB;
		else if (depth && !stencil)
			attachment_type = GL_DEPTH_COMPONENT;
		else if (!depth && stencil)
			attachment_type = GL_STENCIL_INDEX;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (!depth && !stencil)
			glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, width, height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		glDeleteRenderbuffers(1, &RBO);
		glDeleteFramebuffers(1, &FBO);
		glDeleteTextures(1, &textureID);
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &quadVAO);
	}

	void ScreenBuffer::bindBuffer() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	void ScreenBuffer::bindDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ScreenBuffer::clearColor(const vec4& clear_color)
	{
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	}

	void ScreenBuffer::clearBuffer(bool color, bool depth, bool stencil)
	{
		GLenum option = 0;
		if (color)
			option = option | GL_COLOR_BUFFER_BIT;
		if (depth)
			option = option | GL_DEPTH_BUFFER_BIT;
		if (stencil)
			option = option | GL_STENCIL_BUFFER_BIT;

		glClear(option);
	}
	
	void ScreenBuffer::initalize(const vec4& clear)
	{
		ScreenBuffer::clearColor(clear);
		ScreenBuffer::clearBuffer(true, true, true);
		ScreenBuffer::enableDepthTest();
	}

	void ScreenBuffer::drawToTexture() const
	{
		bindBuffer();
		ScreenBuffer::initalize(clear_color);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ScreenBuffer::drawToScreen() const
	{
		ScreenBuffer::bindDefaultBuffer();
		ScreenBuffer::clearColor(clear_color);
		ScreenBuffer::clearBuffer(true, false, false);
		
		bind();

		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		unbind();
	}

}