#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RenderResource.h"

namespace ginkgo
{
	
	struct VertexData
	{
		glm::vec3 vertex;
		glm::vec2 uv;
		float tid;
		unsigned int color;
	};

	class IRenderMesh	//Basically is an array of VertexData, but vectors are easier to use
	{
	public:
		virtual inline void addVertex(const glm::vec3& vertex) = 0;
		virtual inline void addIndex(const unsigned int index) = 0;
		virtual inline void addUV(const glm::vec2& uv) = 0;
		virtual inline void setPosition(const glm::vec3& position);
		virtual inline void setVertices(const std::vector<glm::vec3>& vertices) = 0;
		virtual inline void setIndices(const std::vector<unsigned int>& indices) = 0;
		virtual inline void setColor(const glm::vec4& color) = 0;
		virtual inline void setUVs(const std::vector<glm::vec2>& uv) = 0;
	};

	DECLSPEC_RENDER IRenderMesh* createRenderMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<unsigned int>& indices);
	DECLSPEC_RENDER IRenderMesh* createRenderMesh();

}

