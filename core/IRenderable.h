#pragma once

#include "IEntity.h"

namespace ginkgo
{
	class IRenderMesh;

	class IRenderable
	{
	public:
		virtual const glm::vec3& getScale() const = 0;
		virtual void setScale(const glm::vec3& scl) = 0;
		virtual IRenderMesh const* getRenderMesh() const = 0;
		virtual void setRenderMesh(IRenderMesh const* mesh) = 0;
		virtual void render(const glm::vec3& pos) = 0;
		virtual IEntity* const getParent() const = 0;
	};

	DECLSPEC_CORE IRenderable* renderableFactory(IRenderMesh const* mesh, const glm::vec3& scl = glm::vec3(1, 1, 1));
}