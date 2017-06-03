#pragma once

#include "RenderResource.h"
#include <gl/glew.h>
#include <glm/glm.hpp>

namespace ginkgo 
{
	class Mesh
	{
	private:
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint size;
		GLuint data_size;
	public:
		Mesh();
		~Mesh();
		void addData(const vector<vec3>& positions, const vector<GLuint>& indices, const vector<vec2>& uvs, const vector<vec3>& normals = vector<vec3>());
		virtual void draw() const;
	};
}
