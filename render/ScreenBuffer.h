#pragma once

#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>


namespace ginkgo {

	class ScreenBuffer : public Shader
	{
	private:
		GLuint FBO; //frame buffer object
		GLuint RBO; //render buffer object
		GLuint textureID; //texture color buffer
		GLuint quadVAO; //rectangle
		GLuint quadVBO; //rectangle
		vec4 clear_color;
	public:
		ScreenBuffer(unsigned int screenWidth, unsigned int screenHeight, vec4 clear_color, bool depth, bool stencil);
		~ScreenBuffer();

		void drawToTexture() const;
		void drawToScreen() const;
		static void initalize(const vec4& clearColor = vec4(0, 0, 0, 0));

		void bindBuffer() const;
		static void bindDefaultBuffer();

		static void clearColor(const vec4& clear_color);
		static void clearBuffer(bool colorBuffer, bool depthBuffer, bool stencilBuffer);
		
		static void enableDepthTest() { glEnable(GL_DEPTH_TEST); }
		static void enableStencilTest() { glEnable(GL_STENCIL_TEST); }
		static void disableDepthTest() { glDisable(GL_DEPTH_TEST); }
		static void disableStencilTest() { glDisable(GL_STENCIL_TEST); }
		
		static void drawAsWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		static void drawAsPoints() { glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); }
		static void drawAsFilled() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	};

}