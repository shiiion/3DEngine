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
		airSpeedFactor = 1;

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


	void resolveFreemove(ICharacter& character)
	{
		character.setVelocity(character.getVelocity() * character.getAirSpeedFactor());
	}


	void resolveWalking(ICharacter& character)
	{
		glm::vec3 collisionNormal = character.getPhysics()->getPrimaryCollisionNormal();//TODO find collision normal
		glm::vec3 velocityProjectionOnNormal = (collisionNormal * glm::dot(character.getVelocity(), collisionNormal) / pow(length(collisionNormal), 2));//should work
																																						//checks if slope is away or towards the player's movement, projects velocity on slope
		if (glm::length(character.getVelocity() + collisionNormal) > glm::length(character.getVelocity()))
		{
			character.setVelocity(character.getVelocity() - velocityProjectionOnNormal);
		}
		else
		{
			character.setVelocity(character.getVelocity() + velocityProjectionOnNormal);
		}
	}

	bool isWalkableNormal(glm::vec3 const& normal)
	{
		if (normal.y <= 0)
		{
			return false;
		}
		glm::vec3 normalized = glm::normalize(normal);
		glm::vec3 vertical(0, -1, 0);

		float angBetween = glm::acos(glm::dot(-normalized, vertical));
		return (angBetween <= 45);
	}

	bool checkWalking(ICharacter const& character)
	{
		if (character.getPhysics()->getNumCollisions())//some boolean for collision
		{
			glm::vec3 const& collisionNormal = character.getPhysics()->getPrimaryCollisionNormal();//TODO find collision normal
			return isWalkableNormal(collisionNormal);
		}
		return false;
	} 
}