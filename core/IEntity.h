#pragma once

#include "CoreReource.h"
#include <glm/vec3.hpp>


namespace ginkgo
{
	class IEntity
	{
	public:
		virtual void tick(float elapsedTime) = 0;

		virtual const glm::vec3& getPosition() const = 0;
		virtual const glm::vec3& getVelocity() const = 0;
		virtual const glm::vec3& getAcceleration() const = 0;
		virtual const glm::vec3& getRotation() const = 0;
		virtual long getEntityID() const = 0;

		virtual void setPosition(const glm::vec3& pos) = 0;
		virtual void setVelocity(const glm::vec3& vel) = 0;
		virtual void setAcceleration(const glm::vec3& acc) = 0;
		virtual void setRotation(const glm::vec3& ang) = 0;
		virtual void setEntityID(long ID) = 0;

		virtual EntityType getEntityType() const = 0;
	};
	DECLSPEC IEntity* entityFactory(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
};
