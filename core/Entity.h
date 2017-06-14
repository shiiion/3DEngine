#pragma once
#include "IEntity.h"

namespace ginkgo
{
	class Entity : public IEntity
	{
	private:
		long entityID;

	protected:
		vec3 position;
		quat rotation;
		vec3 velocity;
		vec3 acceleration;
		bool gravityEnabled;//default true

		IPhysicsObject* physicsComponent;
		IRenderComponent* renderComponent;

		vector<IComponent*> componentList;

	public:
		Entity(const vec3& pos, const quat& rot = quat(), const vec3& vel = vec3(), const vec3& accel = vec3());

		virtual void beginTick(float elapsedTime) override;
		virtual void endTick(float elapsedTime) override;

		const vec3& getPosition() const override;
		const vec3& getVelocity() const override;
		const vec3& getAcceleration() const override;
		const quat& getRotation() const override;
		long getEntityID() const override;
		bool isGravityEnabled() const override;
		IRenderComponent* getRenderable() override
		{
			return const_cast<IRenderComponent*>(static_cast<Entity const*>(this)->getRenderable());
		}
		IRenderComponent const* getRenderable() const override
		{
			return renderComponent;
		}
		IPhysicsObject* getPhysics() override
		{
			return const_cast<IPhysicsObject*>(static_cast<Entity const*>(this)->getPhysics());
		}
		IPhysicsObject const* getPhysics() const override
		{
			return physicsComponent;
		}

		void setPosition(const vec3& pos) override;
		void setVelocity(const vec3& vel) override;
		void setAcceleration(const vec3& acc) override;
		void addAcceleration(const vec3& acc) override { acceleration += acc; }
		void setRotation(const quat& ang) override;
		void setEntityID(long ID) override;
		void setRenderable(IRenderComponent* component) override;
		void setPhysics(IPhysicsObject* component) override;
		void setGravityEnabled(bool enabled) override;

		EntityType getEntityType() const override;

		void addComponent(IComponent* component) override;

		virtual ~Entity();
	};
}