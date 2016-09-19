#include "Entity.h"
#include "Core.h"
#include "IPhysicsObject.h"
#include "IWorld.h"

//	Created by the master and nobody else + loser nerd

namespace ginkgo 
{
	Entity::Entity(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel) : allowedMovementStates({1,2})
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;

		entityID = Core::generateID();
		movementState = 0;
		renderableComponent = nullptr;
	}

	const glm::vec3& Entity::getAcceleration() const
	{
		return acceleration;
	}

	const glm::vec3& Entity::getVelocity() const
	{
		return velocity;
	}

	const glm::vec3& Entity::getRotation() const
	{
		return rotation;
	}

	const glm::vec3& Entity::getPosition() const
	{
		return position;
	}

	void Entity::setAcceleration(const glm::vec3& accel)
	{
		acceleration = accel;
	}

	void Entity::setVelocity(const glm::vec3& vel)
	{
		velocity = vel;
	}

	void Entity::setRotation(const glm::vec3& rot)
	{
		rotation = rot;
	}

	void Entity::setPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	void Entity::setEntityID(long ID)
	{
		entityID = ID;
	}

	void Entity::setMovementState(int state)
	{
		this->movementState = state;
	}

	void Entity::beginTick(float elapsedTime)
	{
		if (physicsComponent != nullptr)
		{
			//THE COST OF PREMATURE OPTIMIZATION IS EXPENSIVE
			//if (physicsComponent->getCollisionType() == CTYPE_WORLDSTATIC)
			//{
			//	return;
			//}
			physicsComponent->onTick(elapsedTime);
		}
		position += velocity * elapsedTime;
		velocity += acceleration * elapsedTime;
	}

	void Entity::endTick(float elapsedTime)
	{
		if (physicsComponent != nullptr)
		{
			position = physicsComponent->getMoveResult().finalPos;
			velocity = physicsComponent->getMoveResult().finalVel;
		}
	}

	long Entity::getEntityID() const
	{
		return entityID;
	}

	int Entity::getMovementState() const
	{
		return this->movementState;
	}

	std::vector<int>& Entity::getAllowedMovementStates()
	{
		return this->allowedMovementStates;
	}

	const std::vector<int>& Entity::getAllowedMovementStates() const
	{
		return this->allowedMovementStates;
	}

	EntityType Entity::getEntityType() const
	{
		if (physicsComponent == nullptr)
		{
			return entity;
		}
		return physicsObject;
	}

	IRenderable* Entity::getRenderable() const
	{
		return renderableComponent;
	}

	void Entity::setRenderable(IRenderable* component)
	{
		renderableComponent = component;
	}

	IPhysicsObject* Entity::getPhysics() const
	{
		return physicsComponent;
	}

	void Entity::setPhysics(IPhysicsObject* component)
	{
		physicsComponent = component;
	}

	IEntity* entityFactory(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		return new Entity(pos, rot, vel, accel);
	}
}