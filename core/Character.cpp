#include "Character.h"
#include "IPhysicsObject.h"
#include "IComponent.h"
#include "UserInputSystem.h"
#include <GLFW\glfw3.h>

namespace ginkgo
{
	Character::Character(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;
		airSpeedFactor = 1;
		gravityEnabled = true;

		entityID = Core::generateID();
		renderableComponent = nullptr;

		//freemove
		movementStateList.emplace_back(0);
		//walking
		movementStateList.emplace_back(1);
		movementState = 0;
		
		inputSystem = createUserInputSystem();

		inputSystem->addCommand(Command(1, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(FWD_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(FWD_MOVE);
			}
		}));
		inputSystem->addCommand(Command(2, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(BACK_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(BACK_MOVE);
			}
		}));
		inputSystem->addCommand(Command(3, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(LEFT_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(LEFT_MOVE);
			}
		}));
		inputSystem->addCommand(Command(4, [](IAbstractInputSystem* inputSystem, int outputCode, bool set)
		{
			if (set)
			{
				inputSystem->getOwner()->setMovementControlFlag(RIGHT_MOVE);
			}
			else
			{
				inputSystem->getOwner()->resetMovementControlFlag(RIGHT_MOVE);
			}
		}));

		inputSystem->bindInputCode(GLFW_KEY_W, 1);
		inputSystem->bindInputCode(GLFW_KEY_S, 2);
		inputSystem->bindInputCode(GLFW_KEY_A, 3);
		inputSystem->bindInputCode(GLFW_KEY_D, 4);

		registerInputSystem(inputSystem, this);
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

	void applyMovement(ICharacter& character, float elapsedTime)
	{
		if (character.getMovementControlFlags() & FWD_MOVE)
		{
			character.setVelocity(character.getVelocity() + glm::vec3(0, 0, 1) * elapsedTime);
		}
		if (character.getMovementControlFlags() & BACK_MOVE)
		{
			character.setVelocity(character.getVelocity() + glm::vec3(0, 0, -1) * elapsedTime);
		}
		if (character.getMovementControlFlags() & LEFT_MOVE)
		{
			character.setVelocity(character.getVelocity() + glm::vec3(-1, 0, 0) * elapsedTime);
		}
		if (character.getMovementControlFlags() & RIGHT_MOVE)
		{
			character.setVelocity(character.getVelocity() + glm::vec3(1, 0, 0) * elapsedTime);
		}
	}

	void resolveFreemove(ICharacter& character, float elapsedTime)
	{
		applyMovement(character, elapsedTime);
	}


	void resolveWalking(ICharacter& character, float elapsedTime)
	{
		applyMovement(character, elapsedTime);
		glm::vec3 collisionNormal = character.getPhysics()->getPrimaryCollisionNormal();//TODO: change this to most recent viable collision normal
		glm::vec3 velocityProjectionOnNormal = (collisionNormal * glm::dot(character.getVelocity(), collisionNormal) / pow(length(collisionNormal), 2));//should work
																																						//checks if slope is away or towards the player's movement, projects velocity on slope
		if (glm::length(character.getVelocity() + collisionNormal) > glm::length(character.getVelocity()))
		{
			character.setVelocity(character.getVelocity() - velocityProjectionOnNormal);//TODO: only project movement velocity
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

	bool checkWalking(ICharacter const& character, float elapsedTime)
	{
		if (character.getPhysics()->getNumCollisions())//some boolean for collision
		{
			//TODO: default to newest walkable normal!!
			glm::vec3 const& collisionNormal = character.getPhysics()->getPrimaryCollisionNormal();//TODO find collision normal
			return isWalkableNormal(collisionNormal);
		}
		return false;
	} 
}