#pragma once

#include "Mesh.h"
#include "ObjLoader.h"

namespace ginkgo {

	Mesh::Mesh()
	{
		size = 0;
		data_size = 0;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void Mesh::addData(const vector<vec3>& positions, const vector<GLuint>& indices, const vector<vec2>& uvs, const vector<vec3>& normalsM)
	{
		//Generating Normals
		vector<vec3> normals = normalsM;
		if (normals.size() == 0)
		{
			for (unsigned int i = 0; i < positions.size(); i++)
				normals.emplace_back(vec3(0.0f, 0.0f, 0.0f));

			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				int i0 = indices[i];
				int i1 = indices[i + 1];
				int i2 = indices[i + 2];

				vec3 v1 = positions[i1] - positions[i0];
				vec3 v2 = positions[i2] - positions[i0];

				vec3 normal = normalize(cross(v1, v2));

				//		cout << normal.x << " " << normal.y << " " << normal.z << endl;
				normals[i0] += normal;
				normals[i1] += normal;
				normals[i2] += normal;
			}
			for (unsigned int i = 0; i < normals.size(); i++)
				normals[i] = normalize(normals[i]);
		}

		//Loading Data
		data_size = positions.size() * 8;
		GLfloat* data = new GLfloat[data_size];

		if (positions.size() != uvs.size() || positions.size() != normals.size())
		{
			for (GLuint i = 0; i < positions.size(); i++)
			{
				data[i * 8 + 0] = positions[i].x;
				data[i * 8 + 1] = positions[i].y;
				data[i * 8 + 2] = positions[i].z;
				data[i * 8 + 3] = 0;
				data[i * 8 + 4] = 0;
				data[i * 8 + 5] = normals[i].x;
				data[i * 8 + 6] = normals[i].y;
				data[i * 8 + 7] = normals[i].z;
			}

		}

		for (GLuint i = 0; i < positions.size(); i++)
		{
			data[i * 8 + 0] = positions[i].x;
			data[i * 8 + 1] = positions[i].y;
			data[i * 8 + 2] = positions[i].z;
			data[i * 8 + 3] = uvs[i].x;
			data[i * 8 + 4] = uvs[i].y;
			data[i * 8 + 5] = normals[i].x;
			data[i * 8 + 6] = normals[i].y;
			data[i * 8 + 7] = normals[i].z;
		}

		//Binding Data
		size = indices.size();

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(GLfloat), data, GL_STATIC_DRAW); //&data[0]
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW); //TODO: indices //? too

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		delete[] data;
	}

	void Mesh::draw() const
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}