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
		vec3 rotation;
		vec3 angularVelocity;
		vec3 velocity;
		vec3 acceleration;
		bool gravityEnabled;//default true

		IPhysicsComponent* physics;

		vector<IComponent*> componentList;

		MoveResult finalMove;

	public:
		Entity(const vec3& pos, const vec3& rot = vec3(), const vec3& vel = vec3(), const vec3& accel = vec3());

		virtual void beginTick(float elapsedTime) override;
		virtual void endTick(float elapsedTime) override;

		const vec3& getPosition() const override;
		const vec3& getVelocity() const override;
		const vec3& getAcceleration() const override;
		quat getRotationQ() const override;
		const vec3& getRotation() const override;
		long getEntityID() const override;
		bool isGravityEnabled() const override;

		void setPosition(const vec3& pos) override;
		void setVelocity(const vec3& vel) override;
		void setAcceleration(const vec3& acc) override;
		void addAcceleration(const vec3& acc) override { acceleration += acc; }
		void setRotationQ(const quat& ang) override;
		void setRotation(const vec3& ang) override;
		void setEntityID(long ID) override;
		void setGravityEnabled(bool enabled) override;

		EntityType getEntityType() const override;

		void addComponent(IComponent* component) override;

		IPhysicsComponent* getPhysics() override;

		virtual ~Entity();
	};
}