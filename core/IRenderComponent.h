#pragma once

#include "IEntity.h"
#include "IComponent.h"

namespace ginkgo
{
	class IRenderable;

	class IRenderComponent : public IComponent
	{
	public:
		virtual const vec3& getScale() const = 0;
		virtual void setScale(const vec3& scl) = 0;
		virtual const vec3& getPosition() const = 0;
		virtual void setPosition(const vec3& pos) = 0;

		virtual void setRotation(quat const& rotation) = 0;
		virtual quat const& getRotation() const = 0;

	};

	DECLSPEC_CORE IRenderComponent* renderComponentFactory(IEntity* parent, IRenderable* renderable);
}