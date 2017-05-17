#pragma once

#include <glm/glm.hpp>
#include "IRenderable.h"

namespace ginkgo
{
	class Transform;

	class ILayer
	{
	public:
		virtual unsigned int getSize() const = 0;

		virtual void addRenderable(IRenderable* renderable) = 0;
		virtual IRenderable* alterRenderable(unsigned int index) const = 0;
		virtual const IRenderable* getRenderable(unsigned int index) const = 0;

		virtual const glm::mat4& getModel() const = 0;
		virtual Transform& alterModel() = 0;

		virtual void draw(const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const = 0;

	};
}