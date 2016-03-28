#include "Entity.h"

ginkgo::Entity::Entity(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
{
	position = pos;
	rotation = rot;
	velocity = vel;
	acceleration = accel;
}

ginkgo::Entity::getAcceleration()