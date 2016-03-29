#include "Entity.h"

//	Created by the master and nobody else + loser nerd

namespace ginkgo 
{
	Entity::Entity(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		position = pos;
		rotation = rot;
		velocity = vel;
		acceleration = accel;
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

	void Entity::tick(float elapsedTime)
	{
		position += velocity * elapsedTime;
		velocity += acceleration * elapsedTime;
	}

	IEntity* entityFactory(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3())
	{
		return new Entity(pos, rot, vel, accel);
	}

}