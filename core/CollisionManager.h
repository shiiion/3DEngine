#pragma once

#include "ICollisionManager.h"

namespace ginkgo
{
	class CollisionManager : public ICollisionManager
	{
	private:
		vector<ManifoldData> collisions;


	public:
		CollisionManager();

		bool testCollision(IPhysicsComponent* pcA, IPhysicsComponent* pcB) const override;

		void generateManifold(ManifoldData& manifoldInOut) const override;


		~CollisionManager();
	};


}