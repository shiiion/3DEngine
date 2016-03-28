#pragma once

#include "CoreReource.h"
#include <glm/vec3.hpp>
#include <cstdio>



namespace ginkgo
{
	class IEntity
	{
	public:
		virtual void tick(float elapsedTime) = 0;

		virtual const glm::vec3& getPosition() const = 0;
		virtual const glm::vec3& getVelocity() const = 0;
		virtual const glm::vec3& getAcceleration() const = 0;
		virtual const glm::vec3& getAngles() const = 0;

		virtual void setPosition(const glm::vec3& pos) = 0;
	};
	
};
