#include "Character.h"
#include "IPhysicsObject.h"
#include "IComponent.h"

namespace ginkgo
{
	Character::Character(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;

		entityID = Core::generateID();
		renderableComponent = nullptr;

		//freemove
		movementStateList.emplace_back(0);
		//walking
		movementStateList.emplace_back(1);
		movementState = 0;

		inputSystem = nullptr;
	}

	void Character::beginTick(float elapsedTime)
	{
		for (IComponent* component : componentList)
		{
			component->onTick(elapsedTime);
		}
		if (physicsComponent != nullptr)
		{
			if (physicsComponent->getCollisionType() == CTYPE_WORLDSTATIC)
			{
				return;
			}
			physicsComponent->onTick(elapsedTime);
		}
		position += velocity * elapsedTime;
		//TODO: get rid of getGravity()
		velocity += acceleration * elapsedTime + getWorld()->getGravity() * elapsedTime;
	}

	void Character::endTick(float elapsedTime)
	{
		for (IComponent* component : componentList)
		{
			component->onTickEnd(elapsedTime);
		}
		if (physicsComponent != nullptr)
		{
			position = physicsComponent->getMoveResult().finalPos;
			velocity = physicsComponent->getMoveResult().finalVel;
		}
	}

	void Character::addComponent(IComponent* component)
	{
		componentList.emplace_back(component);
	}

	ICharacter* characterFactory(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		return new Character(pos, rot, vel, accel);
	}
}