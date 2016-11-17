#pragma once
#include "IEntity.h"

namespace ginkgo
{
	class Entity : public IEntity
	{
	private:
		long entityID;

	protected:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		bool gravityEnabled;//default true

		IRenderable* renderableComponent;
		IPhysicsObject* physicsComponent;

		vector<IComponent*> componentList;

	public:
		Entity(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());

		virtual void beginTick(float elapsedTime) override;
		virtual void endTick(float elapsedTime) override;

		const glm::vec3& getPosition() const override;
		const glm::vec3& getVelocity() const override;
		const glm::vec3& getAcceleration() const override;
		const glm::vec3& getRotation() const override;
		long getEntityID() const override;
		bool isGravityEnabled() const override;
		IRenderable* getRenderable() const override;
		IPhysicsObject* getPhysics() const override;

		void setPosition(const glm::vec3& pos) override;
		void setVelocity(const glm::vec3& vel) override;
		void setAcceleration(const glm::vec3& acc) override;
		void addAcceleration(const glm::vec3& acc) override { acceleration += acc; }
		void setRotation(const glm::vec3& ang) override;
		void setEntityID(long ID) override;
		void setRenderable(IRenderable* component) override;
		void setPhysics(IPhysicsObject* component) override;
		void setGravityEnabled(bool enabled) override;

		EntityType getEntityType() const override;

		void addComponent(IComponent* component) override;
	};
}