#pragma once

#include <vector>
#include <GL/glew.h>

#include "Buffer.h"

namespace ginkgo 
{

	class VertexArray
	{
	private:
		GLuint arrayID;
		std::vector<Buffer*> buffers;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};

}