#pragma once

#include <GL/glew.h>

#include "IRenderEngine.h"
#include "buffers/IndexBuffer.h"

namespace ginkgo
{

	class RenderEngine : public IRenderEngine
	{
	private:
		GLuint VAO;
		GLuint VBO;
		IndexBuffer* IBO;
		GLsizei indexCount;
		VertexData* buffer;
	public:
		RenderEngine();
		~RenderEngine();
	private:
		void init() override;
	public:
		void begin() override;
		void submit(const RenderMesh* mesh) override;
		void end() override;
		void flush() override;

	};


}

