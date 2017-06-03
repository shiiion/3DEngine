#pragma once


#include "IText.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include <ft2build.h>
#include <freetype\freetype.h>


namespace ginkgo {

	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

	class Text : public Shader, public IText
	{
	private:
		unsigned int VAO;
		unsigned int VBO;
		map<GLchar, Character> Characters;
		float maxWidth;
		float maxHeight;
		float minWidth;
		float minHeight;
	public:
		Text(float windowWidth, float windowHeight, const char* fontFilePath, unsigned int fontSize);
		~Text();

		void draw(const string& text, GLfloat x, GLfloat y, GLfloat scale, const vec3& color) override;

		float getMaxCharWidth() const override { return maxWidth; }
		float getMaxCharHeight() const override { return maxHeight; }
		float getMinCharWidth() const override { return minWidth; }
		float getMinCharHeight() const override { return minHeight; }

	};

}