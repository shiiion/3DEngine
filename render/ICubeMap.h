#pragma once

namespace ginkgo
{
	class ICubeMap
	{
	public:
		virtual void draw(const glm::mat4& transformProjectionView) const = 0;
		virtual void bindCubeMapTexture() const = 0;
		virtual void unbindCubeMapTexture() const = 0;
	};

	ICubeMap* cubeMapFactory(std::map<unsigned int, std::string> faces, float scale = 500.0f);
}