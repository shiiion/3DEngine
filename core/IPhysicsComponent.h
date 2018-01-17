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
		virtual void setMaterial(const PhysMaterial& mat) = 0;
		virtual void setMass(float mass) = 0;
		virtual void setCollisionMesh(ICollisionMesh* collision) = 0;
		virtual void setMovementState(UINT32 state) = 0;

		virtual const PhysMaterial& getMaterial() const = 0;
		virtual float getMass() const = 0;
		virtual ICollisionMesh* getCollisionMesh() const = 0;
		virtual UINT32 getCollisionType() const = 0;

		virtual ~IPhysicsComponent() = 0;
	};

	DECLSPEC_CORE IPhysicsComponent* PhysicsComponentFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat);
}