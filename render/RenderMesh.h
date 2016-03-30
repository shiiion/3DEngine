#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace ginkgo
{

	struct VertexData
	{
		glm::vec3 vertex;
		glm::vec2 uv;
		float tid;
		unsigned int color;
	};

	class RenderMesh	//Basically is an array of VertexData, but vectors are easier to use
	{
	private:
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> uvs;
		glm::vec4 color;
		//TODO: Later change glm::vec4 color to std::vector<glm::vec4> for each vertex
	public:
		inline void addVertex(const glm::vec3& vertex)	{	vertices.push_back(vertex);	};
		inline void addIndex(const unsigned int index) { indices.push_back(index);	}
		inline void addUV(const glm::vec2& uv) { uvs.push_back(uv);	}
		inline void setVertices(std::vector<glm::vec3>& vertices)	{	this->vertices = vertices;	}
		inline void setIndices(std::vector<unsigned int>& indices)	{	this->indices = indices;	}
		inline void setColor(glm::vec4& color) { this->color = color; }
	};


}

