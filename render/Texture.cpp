#include <iostream>

#include "Texture.h"

#include "FileUtils.h"

namespace ginkgo {

	std::string Texture::whitepixelfilepath = "Render/res/textures/white.png";

	Texture::Texture(const std::string& path, bool pixelate)
		: filepath(path)
	{
		BYTE* pixels = FileUtils::loadImage(filepath.c_str(), &width, &height);
		GLenum err;
		err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cout << err;
			std::cout << "Error loading texture. 1" << std::endl;
			system("pause");

		}
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		GLenum option = (pixelate) ? GL_NEAREST : GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		err = glGetError();
		glBindTexture(GL_TEXTURE_2D, 0);
		if (err != GL_NO_ERROR)
		{
			std::cout << err << std::endl << tid << std::endl;
			std::cout << "Error loading texture. 2" << std::endl;
			system("pause");
		}

		delete[] pixels;
	}


	Texture::Texture(const std::string& path)
		: Texture(path, false)
	{ }
	
	Texture::~Texture()
	{ 
		glDeleteTextures(1, &tid);
	}

	const void Texture::setID(unsigned int texureID)
	{
		tid = texureID;
	}

	const void Texture::deleteTexture() const
	{
		glDeleteTextures(1, &tid);
	}

}