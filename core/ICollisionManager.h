#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	// singleton
	class ICollisionManager
	{
	public:
		virtual bool testCollision(IPhysicsComponent* colliderA, IPhysicsComponent* colliderB) const = 0;
		
		// Precondition: colliders are colliding
		// Provided: thisMesh, otherMesh, collisionTime
		// Output: overlapNormal, overlapDist, contactA, contactB
		virtual void generateManifold(ManifoldData& manifoldInOut) const = 0;
		
		
		virtual ~ICollisionManager() = 0;
	};

	DECLSPEC_CORE ICollisionManager* getCollisionManager();
}