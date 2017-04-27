#include <FreeImage\FreeImage.h>

#include "CubeMap.h"

#include "FileUtils.h"
#include "Transform.h"

namespace ginkgo {

	CubeMap::CubeMap(std::map<unsigned int, std::string> faces, float scale)
	{
		addVertexShader("Render/res/shaders/cubemapVertex.vs");
		addFragmentShader("Render/res/shaders/cubemapFragment.fs");
		compileShader();

		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);

		int width, height;
		unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		//RIGHT FACE
		image = FileUtils::loadImage(faces[CubeMap::RIGHT].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::LEFT, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;

		//LEFT FACE
		image = FileUtils::loadImage(faces[CubeMap::LEFT].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::RIGHT, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;

		//TOP FACE
		image = FileUtils::loadImage(faces[CubeMap::TOP].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::TOP, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;

		//BOTTOM FACE
		image = FileUtils::loadImage(faces[CubeMap::BOTTOM].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::BOTTOM, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;

		//FRONT FACE
		image = FileUtils::loadImage(faces[CubeMap::FRONT].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::BACK, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;

		//BACK FACE
		image = FileUtils::loadImage(faces[CubeMap::BACK].c_str(), &width, &height, 180.0f);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + CubeMap::FRONT, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
		delete[] image;


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		GLfloat vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

		for (unsigned int i = 0; i < 108; i++)
			vertices[i] *= scale;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	CubeMap::~CubeMap() {
		glDeleteTextures(1, &textureID);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void CubeMap::bindCubeMapTexture() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}

	void CubeMap::unbindCubeMapTexture() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void CubeMap::draw(const glm::mat4& transformProjectionView) const
	{
		bind();

		glDepthFunc(GL_LEQUAL);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		setUniformMat4("transform", transformProjectionView);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);
		
		glDepthFunc(GL_LESS);

		unbind();
	}

}