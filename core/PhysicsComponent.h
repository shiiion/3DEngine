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
		bool canCollide;
		UINT32 numCollisions;
		UINT32 movementState;
		UINT32 collisionType;

		IEntity* const parent;

		MoveResult finalMove;

		std::forward_list<SurfaceData> normalList;
		quat rotationBuffer;

	public:
		PhysicsComponent(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide = true);

		bool checkCollision(float deltaTime, IPhysicsComponent* other) override;

		void setMaterial(const PhysMaterial& mat) override;
		void setMass(float mass) override;
		void setCanCollide(bool collides) override;
		void setCollisionMesh(ICollisionMesh* collision) override;
		void incrementCollision() override;
		void decrementCollision() override;
		void setMovementState(UINT32 state) override;
		void setRotation(const quat& rotation) override;

		const PhysMaterial& getMaterial() const override;
		float getMass() const override;
		bool doesCollide() const override;
		ICollisionMesh* getCollisionMesh() const override;
		UINT32 getNumCollisions() const override;
		UINT32 getCollisionType() const override;

		bool isMoving() const override;

		void setFinalMove(MoveResult const& finalMove) override;

		void onTick(float elapsedTime) override;

		IEntity* const getParent() const override;

		const MoveResult& getMoveResult() const override;
		
		std::forward_list<SurfaceData> const& getCollisionNormalList() const override;
		void removeNormal(SurfaceData const& data) override;
	};
}