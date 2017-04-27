#pragma once

#include <map>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <ft2build.h>
#include <freetype\freetype.h>

#include "Shader.h"

namespace ginkgo {

	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

	class Text : public Shader
	{
	private:
		unsigned int VAO;
		unsigned int VBO;
		std::map<GLchar, Character> Characters;
		float maxWidth;
		float maxHeight;
		float minWidth;
		float minHeight;
	public:
		Text(float windowWidth, float windowHeight, const char* fontFilePath);
		~Text();

		void draw(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color);

		float getMaxCharWidth() const { return maxWidth; }
		float getMaxCharHeight() const { return maxHeight; }
		float getMinCharWidth() const { return minWidth; }
		float getMinCharHeight() const { return minHeight; }

	};

}