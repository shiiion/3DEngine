#pragma once
#include <string>
#include <GL/glew.h>

namespace ginkgo
{
	class IText
	{
		virtual void draw(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color) = 0;

		virtual float getMinCharWidth() const = 0;
		virtual float getMaxCharWidth() const = 0;
		virtual float getMaxCharHeight() const = 0;
		virtual float getMinCharHeight() const = 0;
	};
}