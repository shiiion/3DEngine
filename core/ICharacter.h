#pragma once

#include "IPhysicsObject.h"

namespace ginkgo
{
	class IAbstractInputSystem;

	class ICharacter : public IPhysicsObject
	{
	public:
		virtual void setInputSystem(IAbstractInputSystem* inputSystem) = 0;
		virtual IAbstractInputSystem* getInputSystem() const = 0;


	};
}