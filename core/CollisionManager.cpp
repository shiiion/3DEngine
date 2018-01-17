#include "CollisionManager.h"

namespace ginkgo
{
	static ICollisionManager* sCollisionMgr = nullptr;

	CollisionManager::CollisionManager()
	{

	}

	bool CollisionManager::testCollision(IPhysicsComponent* pcA, IPhysicsComponent* pcB) const
	{
		return false;
	}

	void CollisionManager::generateManifold(ManifoldData& manifoldInOut) const
	{

	}

	ICollisionManager::~ICollisionManager() {}

	CollisionManager::~CollisionManager() {}

	ICollisionManager* getCollisionManager()
	{
		if (!sCollisionMgr)
		{
			sCollisionMgr = new CollisionManager();
		}
		return sCollisionMgr;
	}
}