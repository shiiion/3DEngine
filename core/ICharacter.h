#pragma once

#include "IEntity.h"

namespace ginkgo
{
	class IAbstractInputSystem;

	class ICharacter : public IEntity
	{
	public:
		virtual void setInputSystem(IAbstractInputSystem* inputSystem) = 0;
		virtual IAbstractInputSystem const* getInputSystem() const = 0;
		virtual IAbstractInputSystem* getInputSystem() = 0;
		virtual vector<int> const& getMovementStates() const = 0;
		virtual void addMovementState(int movementStateID) = 0;
		virtual void setMovementState(int movementStateID) = 0;
		virtual int getMovementState() const = 0;

		virtual float getAirSpeedFactor() const = 0;
		virtual void setAirSpeedFactor(float factor) = 0;
		virtual void setMovementControlFlag(int flag) = 0;
		virtual void resetMovementControlFlag(int flag) = 0;

		virtual int getMovementControlFlags() const = 0;
	};

	DECLSPEC_CORE ICharacter* characterFactory(const vec3& pos, const quat& rot = quat(), const vec3& vel = vec3(), const vec3& accel = vec3());
}