#pragma once 

#include "RenderResource.h"

#include <GL/glew.h>
#include <FreeImage/FreeImage.h>

namespace ginkgo {

	class FileUtils
	{
	public:
		static string read_file(const char* filepath);
		static unsigned char* loadImage(const char* filename, GLsizei* width, GLsizei* height, double rotationAngleInDegrees = 0);
		static void screenshot(unsigned int width, unsigned int height);
	};

}