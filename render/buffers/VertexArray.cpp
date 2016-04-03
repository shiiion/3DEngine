#include "VertexArray.h"

namespace ginkgo
{

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &arrayID);
	}

	VertexArray::~VertexArray()
	{
		for (unsigned int i = 0; i < buffers.size(); i++)
			delete buffers[i];

		glDeleteVertexArrays(1, &arrayID);
	}

	void VertexArray::addBuffer(Buffer* buffer, GLuint index)
	{
		bind();
		buffer->bind();

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);

		buffer->unbind();
		unbind();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(arrayID);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}