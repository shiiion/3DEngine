#include "Entity.h"

//	Created by the master and nobody else + loser nerd

ginkgo::Entity::Entity(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
{
	position = pos;
	rotation = rot;
	velocity = vel;
	acceleration = accel;
}

const glm::vec3& ginkgo::Entity::getAcceleration() const
{
	return acceleration;
}

const glm::vec3& ginkgo::Entity::getVelocity() const
{
	return velocity;
}

const glm::vec3& ginkgo::Entity::getRotation() const
{
	return rotation;
}

const glm::vec3& ginkgo::Entity::getPosition() const
{
	return position;
}

void ginkgo::Entity::setAcceleration(const glm::vec3& accel)
{
	acceleration = accel;
}

void ginkgo::Entity::setVelocity(const glm::vec3& vel)
{
	velocity = vel;
}

void ginkgo::Entity::setRotation(const glm::vec3& rot)
{
	rotation = rot;
}

void ginkgo::Entity::setPosition(const glm::vec3& pos)
{
	position = pos;
}

void ginkgo::Entity::tick(float elapsedTime)
{
	position += velocity * elapsedTime;
	velocity += acceleration * elapsedTime;
}