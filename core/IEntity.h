#pragma once

#include "CoreReource.h"


namespace ginkgo
{
	class IRenderable;
	class IComponent;

	class IEntity
	{
	public:
		virtual void beginTick(float elapsedTime) = 0;
		virtual void endTick(float elapsedTime) = 0;

		virtual const glm::vec3& getPosition() const = 0;
		virtual const glm::vec3& getVelocity() const = 0;
		virtual const glm::vec3& getAcceleration() const = 0;
		virtual const glm::vec3& getRotation() const = 0;
		virtual long getEntityID() const = 0;
		virtual bool isGravityEnabled() const = 0;

		virtual void setPosition(const glm::vec3& pos) = 0;
		virtual void setVelocity(const glm::vec3& vel) = 0;
		virtual void setAcceleration(const glm::vec3& acc) = 0;
		virtual void addAcceleration(const glm::vec3& acc) = 0;
		virtual void setRotation(const glm::vec3& ang) = 0;
		virtual void setEntityID(long ID) = 0;
		virtual void setGravityEnabled(bool enabled) = 0;

		virtual EntityType getEntityType() const = 0;

		virtual IRenderable* getRenderable() const = 0;
		virtual IPhysicsObject* getPhysics() const = 0;
		virtual void setRenderable(IRenderable* component) = 0;
		virtual void setPhysics(IPhysicsObject* component) = 0;

		virtual void addComponent(IComponent* component) = 0;
	};
	DECLSPEC_CORE IEntity* entityFactory(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
};
