#pragma once

#include "IEntity.h"

namespace ginkgo
{
	class IAbstractInputSystem;

	class ICharacter : public IEntity
	{
	public:
		virtual void setInputSystem(IAbstractInputSystem* inputSystem) = 0;
		virtual IAbstractInputSystem* getInputSystem() const = 0;
		virtual vector<int> const& getMovementStates() const = 0;
		virtual void addMovementState(int movementStateID) = 0;
		virtual void setMovementState(int movementStateID) = 0;
		virtual int getMovementState() const = 0;
	};

	DECLSPEC_CORE ICharacter* characterFactory(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
}