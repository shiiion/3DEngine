#pragma once

#include "RenderResource.h"
#include <GL/glew.h>

namespace ginkgo
{
	class IText
	{
	public:
		virtual void draw(const string& text, GLfloat x, GLfloat y, GLfloat scale, const vec3& color) = 0;

		virtual float getMinCharWidth() const = 0;
		virtual float getMaxCharWidth() const = 0;
		virtual float getMaxCharHeight() const = 0;
		virtual float getMinCharHeight() const = 0;
	};

	DECLSPEC_RENDER IText* textRenderFactory(float windowWidth, float windowHeight, const char* fontFilePath, unsigned int fontSize);
}