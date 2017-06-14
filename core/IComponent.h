#pragma once

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
	};
}