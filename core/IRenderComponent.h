#pragma once

#include "IEntity.h"
#include "IComponent.h"

namespace ginkgo
{
	class IRenderable;

	class IRenderComponent : public IComponent
	{
	public:
		virtual const glm::vec3& getScale() const = 0;
		virtual void setScale(const glm::vec3& scl) = 0;
	};

	DECLSPEC_CORE IRenderComponent* renderableFactory(IEntity* parent, IRenderable const* renderable);
}