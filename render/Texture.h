#pragma once

#include "RenderResource.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <FreeImage/FreeImage.h>

namespace ginkgo {

	struct Texture
	{
	public:
		string filepath;
		GLuint tid;
		GLsizei width;
		GLsizei height;
		static string whitepixelfilepath;
	public:
		~Texture()
		{
			glDeleteTextures(1, &tid);
		}
	};
}


/*
images -> no alpha, remove transparency in Gimp Layer->Transparency->Remove Alpha Channel
*/
