#pragma once

#include "IPhysicsComponent.h"
#include "Entity.h"

namespace ginkgo
{
	class PhysicsComponent : public IPhysicsComponent
	{
	private:
		ICollisionMesh* collisionMesh;
		PhysMaterial material;
		float mass;

		UINT32 movementState;
		UINT32 collisionType;

		IEntity* parent;

		MoveResult moveBuffer;

	public:
		PhysicsComponent(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat);

		void setMaterial(const PhysMaterial& mat) override;
		void setMass(float mass) override;
		void setCollisionMesh(ICollisionMesh* collision) override;
		void setMovementState(UINT32 state) override;

		const PhysMaterial& getMaterial() const override;
		float getMass() const override;
		ICollisionMesh* getCollisionMesh() const override;
		UINT32 getCollisionType() const override;

		void onTick(float elapsedTime) override;
		void onTickEnd(float elapsedTime) override;

		void onDetach() override;
		
		int type() const override;

		IEntity* getParent() override;
	};
}