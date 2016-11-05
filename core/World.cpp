#include "World.h"
#include "IEntity.h"
#include "IRenderable.h"
#include "IPhysicsObject.h"
#include "SurfaceCollisionMesh.h"

namespace ginkgo
{

	World::World(float gravity)
		: worldTree(0, Prism(WORLD_DIMENSIONS), nullptr)
	{
		this->gravity = glm::vec3(0, gravity, 0);
	}

	const vector<IEntity*>& World::getEntityList() const
	{
		return entityList;
	}

	vector<IEntity*> World::getEntitiesByType(EntityType type) const
	{
		vector<IEntity*> newEntityList;
		if (type == entity)
		{
			for (UINT32 a = 0; a < entityList.size(); a++)
			{
				newEntityList.at(a) = entityList.at(a);
			}
			return newEntityList;

		}
		else if (type == physicsObject)
		{
			for (UINT32 a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType() >= physicsObject)
				{
					newEntityList.push_back(entityList.at(a));
				}
			}
			return newEntityList;
		}
		else
			return newEntityList;
	}

	void World::clearWorld()
	{
		for (UINT32 a = 0; a < entityList.size(); a++)
		{
			delete entityList.at(a);
		}
		entityList.clear();
		worldTree.resetTree(0, Prism(WORLD_DIMENSIONS));
	}

	void World::setGravity(float gravity)
	{
		this->gravity = glm::vec3(0, gravity, 0);
	}

	glm::vec3 const& World::getGravity() const
	{
		return gravity;
	}

	IEntity* World::getEntity(long ID) const
	{
		UINT32 a;
		for (a = 0; a < entityList.size(); a++)
		{
			if (entityList.at(a)->getEntityID() == ID)
				break;
		}
		if (a == entityList.size())
			return nullptr;
		return entityList.at(a);
	}

	void World::addEntity(IEntity* entity)
	{
		entityList.push_back(entity);
		if (entity->getEntityType() >= physicsObject)
		{
			worldTree.insert(entity->getPhysics());
		}
	}

	void World::removeEntity(long ID)
	{
		UINT32 a;
		for (a = 0; a < entityList.size(); a++)
		{
			if (entityList.at(a)->getEntityID() == ID)
				break;
		}
		if (a == entityList.size())
		{
			return;
		}
		if (entityList[a]->getEntityType() == physicsObject)
		{
			worldTree.remove(ID);
		}
		delete entityList.at(a);
	}

	void World::traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut)
	{
		resultOut.ray = ray;
		resultOut.rayDist = dist;

		vector<IPhysicsObject*> possibleCollisions;
		worldTree.retrieveCollisions(possibleCollisions, ray, dist);

		for (IPhysicsObject* collider : possibleCollisions)
		{
			if (collider->getCollisionMesh()->testRay(params, resultOut))
			{
				return;
			}
		}
		resultOut.firstCollision = nullptr;
		resultOut.didHit = false;
		resultOut.collisionDist = -1;
	}

	int World::registerMovementState(const std::string & name, const CheckIfMovementState & CheckMovementState, const DoOnMovementState & OnMovementState)
	{
		RegisteredMovementState state(name, CheckMovementState, OnMovementState);
		return manager.RegisterMovementState(state);
	}

	int World::getMovementState(const std::string & name) const
	{
		return manager.GetMovementStateID(name);
	}

	void World::checkMovementStates(float elapsedTime)
	{
		manager.CheckMovementStates(this->entityList, elapsedTime);
	}

	void World::doMovementStates(float elapsedTime)
	{
		manager.DoCallbacks(this->entityList, elapsedTime);
	}

	void World::addCollision(CollisionInfo const& info, float deltaTime)
	{
		collisions.emplace_back(Collision(deltaTime, info));
		info.thisMesh->getOwner()->incrementCollision();
		info.otherMesh->getOwner()->incrementCollision();
	}

	void World::clearCollisionCache()
	{
		for (UINT32 a = 0; a < collisions.size(); a++)
		{
			if (!collisions[a].valid)
			{
				//collisions[a].manifold.thisMesh->getOwner()->setCollisionState(CSTATE_NO)
				collisions[a].manifold.thisMesh->getOwner()->decrementCollision();
				collisions[a].manifold.otherMesh->getOwner()->decrementCollision();
				collisions.erase(collisions.begin() + a);
				a--;
			}
		}
	}

	void World::resolveCollisions(INT32 iterations)
	{
		for (INT32 a = 0; a < iterations; a++)
		{
			for (Collision& c : collisions)
			{
				//skip c if the overlap is less than min overlap
				if (c.preCorrectionCheck())
				{
					if (a == 0)
					{
						c.applyFriction();
					}
					c.positionalCorrection(0.2f);
				}
			}
		}

		for (Collision& c : collisions)
		{
			c.postCorrection();
		}
	}

	bool World::collisionExists(IPhysicsObject* a, IPhysicsObject* b) const
	{
		bool found = false;

		for (Collision const& c : collisions)
		{
			if (found |= c.eq(a, b)) break;
		}
		return found;
	}

	void World::preCollisionTest()
	{
		for (Collision& c : collisions)
		{
			c.referenceResult.finalPos = c.manifold.thisMesh->getCachedCenter();
			c.referenceResult.finalVel = c.manifold.thisMesh->getCachedVelocity();
			c.otherResult.finalPos = c.manifold.otherMesh->getCachedCenter();
			c.otherResult.finalVel = c.manifold.otherMesh->getCachedVelocity();
			c.updateValidity();
		}
		clearCollisionCache();
	}

	void World::updateOctreeIndex(IEntity* entity)
	{
		worldTree.reinsert(entity);
	}
}

