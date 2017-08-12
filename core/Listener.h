#pragma once

#include "IComponent.h"
#include "AudioManager.h"
#include "IEntity.h"

namespace ginkgo
{
	class Listener : public IComponent
	{
	public:
		void onTick(float elapsedTime);

		void onTickEnd(float elapsedTime);

		IEntity* getParent();

		Listener(IEntity* entity);

		void onDetach() {}
	private:
		IEntity* parent;
	};
}