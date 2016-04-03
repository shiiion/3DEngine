#pragma once

#include <GL/glew.h>

namespace ginkgo
{
	class Buffer
	{
	private:
		GLuint bufferID;
		GLuint componentCount;
	public:
		Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
		~Buffer();
		void bind() const;
		void unbind() const;
		inline GLuint getComponentCount() const { return componentCount; }
	};

}
}