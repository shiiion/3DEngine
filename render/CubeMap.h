#pragma once


#include "ICubeMap.h"
#include "Shader.h"
#include "Transform.h"
#include <glm/glm.hpp>

namespace ginkgo {
	
	class Transform;

	class CubeMap : public Shader, public ICubeMap
	{
	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int textureID;
	public:
		CubeMap(map<unsigned int, string> faces, float scale);
		~CubeMap();
		void draw(const mat4& transformProjectionView) const override;
		void bindCubeMapTexture() const override;
		void unbindCubeMapTexture() const override;
	};
	
}