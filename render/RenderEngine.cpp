#include "RenderEngine.h"

namespace ginkgo {

	RenderEngine::RenderEngine()
	{
		init();
	}

	RenderEngine::~RenderEngine()
	{
		delete IBO;
		glDeleteBuffers(1, &VBO);
	}

	void RenderEngine::init()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glEnableVertexAttribArray(SHADER_TID_INDEX);

		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::vertex)));
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));
		glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::tid)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

		glBindVertexArray(0);
	}


	void RenderEngine::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void RenderEngine::submit(const RenderMesh* renderable)
	{
		const glm::vec3& position = renderable->getPosition();
		const glm::vec2& size = renderable->getSize();
		const glm::vec4& color = renderable->getColor();
		const std::vector<glm::vec2>& uv = renderable->getUV();
		const GLuint tid = renderable->getTID();

		unsigned int c = 0;
		float ts = 0.0f;
		//if tid > 0 -> texture, else color
		if (tid > 0)
		{
			bool found = false;
			for (int i = 0; i < TextureSlots.size(); i++)
			{
				if (TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (TextureSlots.size() >= RENDERER_MAX_TEXTURES)
				{
					end();
					flush();
					begin();
				}
				TextureSlots.push_back(tid);
				ts = (float)(TextureSlots.size());
			}

		}
		else
		{
			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;
			c = a >> 24 | b << 16 | g << 8 | r;
		}

		buffer->vertex = position;
		buffer->uv = uv[0];
		buffer->tid = ts;
		buffer->color = c;
		buffer++;

		buffer->vertex = glm::vec3(position.x, position.y + size.y, position.z);
		buffer->uv = uv[1];
		buffer->tid = ts;
		buffer->color = c;
		buffer++;

		buffer->vertex = glm::vec3(position.x + size.x, position.y + size.y, position.z);
		buffer->uv = uv[2];
		buffer->tid = ts;
		buffer->color = c;
		buffer++;

		buffer->vertex = glm::vec3(position.x + size.x, position.y, position.z);
		buffer->uv = uv[3];
		buffer->tid = ts;
		buffer->color = c;
		buffer++;

		indexCount += 6;
	}

	void RenderEngine::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void RenderEngine::flush()
	{

		for (int i = 0; i < TextureSlots.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, TextureSlots[i]);
		}

		glBindVertexArray(VAO);
		IBO->bind();

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

		IBO->unbind();
		glBindVertexArray(0);

		indexCount = 0;
		TextureSlots.clear();
	}

	IRenderEngine* createRenderEngine()
	{
		return new RenderEngine();
	}

}