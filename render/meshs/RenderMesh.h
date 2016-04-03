#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RenderResource.h"
#include "IRenderMesh.h"

namespace ginkgo
{

	class RenderMesh : IRenderMesh	//Basically is an array of VertexData, but vectors are easier to use
	{
	private:
		glm::vec3 position;
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> uvs;
		glm::vec4 color;
		//TODO: Later change glm::vec4 color to std::vector<glm::vec4> for each vertex
	public:
		RenderMesh();
		RenderMesh(const glm::vec3& position, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<unsigned int>& indices)
			:	position(position), vertices(vertices), uvs(uvs), indices(indices)
		{	}
		inline void addVertex(const glm::vec3& vertex)	override {	vertices.push_back(vertex);	}
		inline void addIndex(const unsigned int index) override { indices.push_back(index);	}
		inline void addUV(const glm::vec2& uv) override { uvs.push_back(uv);	}

		inline void setPosition(const glm::vec3& position) override { this->position = position;  }
		inline void setVertices(const std::vector<glm::vec3>& vertices)	override {	this->vertices = vertices;	}
		inline void setIndices(const std::vector<unsigned int>& indices)	override {	this->indices = indices;	}
		inline void setColor(const glm::vec4& color) override { this->color = color; }
		inline void setUVs(const std::vector<glm::vec2>& uv) override { this->uvs = uv; }

		IRenderMesh* createRenderMesh(const glm::vec3& position, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<unsigned int>& indices)
		{
			return new RenderMesh(position, vertices, uvs, indices);
		}
		
		IRenderMesh* createRenderMesh()
		{
			return new RenderMesh();
		}

	};



}

