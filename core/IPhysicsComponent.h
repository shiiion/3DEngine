#pragma once

#include "CoreReource.h"
#include "IComponent.h"
#include <forward_list>

#define CSTATE_RESOLVE 1
#define CSTATE_NOCOLLISION 0

#define MSTATE_AIR 1
#define MSTATE_GROUND 2
//MORE STUFF CAN BE ADDED

//object is never resolved (pushes objects out of the way)
//typically for objects which will never move, such as 1or, wall, etc
#define CTYPE_WORLDSTATIC 1
//object is always resolved (pushed out of the way)
//typically for moving objects in the world
#define CTYPE_WORLDDYNAMIC 2

namespace ginkgo
{
	class ICollisionMesh;
	class IEntity;
	struct Collision;

	class IPhysicsComponent : public IComponent
	{
	public:
		virtual bool checkCollision(float deltaTime, IPhysicsComponent* other) = 0;
		
		virtual void setMaterial(const PhysMaterial& mat) = 0;
		virtual void setMass(float mass) = 0;
		virtual void setCanCollide(bool collides) = 0;
		virtual void setCollisionMesh(ICollisionMesh* collision) = 0;
		virtual void incrementCollision() = 0;
		virtual void decrementCollision() = 0;
		virtual void setMovementState(UINT32 state) = 0;
		virtual void setRotation(const quat& rotation) = 0;

		virtual const PhysMaterial& getMaterial() const = 0;
		virtual float getMass() const = 0;
		virtual bool doesCollide() const = 0;
		virtual ICollisionMesh* getCollisionMesh() const = 0;
		virtual UINT32 getNumCollisions() const = 0;
		virtual UINT32 getCollisionType() const = 0;

		virtual bool isMoving() const = 0;

		virtual void setFinalMove(MoveResult const& finalMove) = 0;

		virtual const MoveResult& getMoveResult() const = 0;
		
		//Returns a list of all collision normals for this object (updated per-tick)
		//Most recent normal is at the beginning of the list
		virtual std::forward_list<SurfaceData> const& getCollisionNormalList() const = 0;

		virtual void removeNormal(SurfaceData const& data) = 0;


		virtual ~IPhysicsComponent() = 0;
	};

	DECLSPEC_CORE IPhysicsComponent* PhysicsComponentFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide = true);
}