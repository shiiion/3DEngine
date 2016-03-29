#pragma once

#include "IEntity.h"

namespace ginkgo
{
	class IRenderMesh;

	class IRenderable : public IEntity
	{
	public:
		virtual const glm::vec3& getScale() const = 0;
		virtual void setScale(const glm::vec3& scl) = 0;
		virtual IRenderMesh const* getRenderMesh() const = 0;
		virtual void setRenderMesh(IRenderMesh const* mesh) = 0;
		virtual void render() = 0;
	};

	DECLSPEC IRenderable* renderableFactory(IRenderMesh const* mesh, const glm::vec3& pos, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
}