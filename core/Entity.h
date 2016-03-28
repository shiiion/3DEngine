#pragma once
#include "IEntity.h"

namespace ginkgo
{
	class Entity : public IEntity
	{
	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 velocity;
		glm::vec3 acceleration;
	
	public:
		Entity(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());

		virtual void tick(float elapsedTime) override;

		virtual const glm::vec3& getPosition() const override;
		virtual const glm::vec3& getVelocity() const override;
		virtual const glm::vec3& getAcceleration() const override;
		virtual const glm::vec3& getRotation() const override;

		virtual void setPosition(const glm::vec3& pos) override;
		virtual void setVelocity(const glm::vec3& vel) override;
		virtual void setAcceleration(const glm::vec3& acc) override;
		virtual void setRotation(const glm::vec3& ang) override;
	};
}