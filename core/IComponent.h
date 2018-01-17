#pragma once

#define PHYSICS_COMPONENT 1
#define RENDER_COMPONENT 2

namespace ginkgo
{
	class IEntity;

	class IComponent
	{
	public:
		virtual void onTick(float elapsedTime) = 0;

		virtual void onTickEnd(float elapsedTime) = 0;

		virtual void onDetach() = 0;

		virtual IEntity* getParent() = 0;

		virtual int type() const = 0;

		virtual ~IComponent() = 0;
	};
}