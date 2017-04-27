#pragma once

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <FreeImage/FreeImage.h>

namespace ginkgo {

	class Texture
	{
	private:
		std::string filepath;
		GLuint tid;
		GLsizei width;
		GLsizei height;
		static std::string whitepixelfilepath;
	public:
		Texture(const std::string& path, bool pixelate);
		Texture(const std::string& path = whitepixelfilepath);
		~Texture();
		const unsigned int getWidth() const { return width; }
		const unsigned int getHeight() const { return height; }
		const GLuint getID() const { return tid; }
		const void setID(unsigned int texureID);
		const void deleteTexture() const;

		static void setWhitePixelFilePath(const std::string& filepath) { whitepixelfilepath = filepath; }
	};

}


/*
images -> no alpha, remove transparency in Gimp Layer->Transparency->Remove Alpha Channel
*/
