#include "World.h"
#include "IEntity.h"
#include "IRenderable.h"
#include "IPhysicsComponent.h"

namespace ginkgo
{

	World::World(float gravity)
		: worldTree(0, Prism(WORLD_DIMENSIONS), nullptr)
	{
		this->gravity = vec3(0, gravity, 0);
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
		else if (type == physicsComponent)
		{
			for (UINT32 a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType() >= physicsComponent)
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
		//worldTree.resetTree(0, Prism(WORLD_DIMENSIONS));
	}

	void World::setGravity(float gravity)
	{
		this->gravity = vec3(0, gravity, 0);
	}

	vec3 const& World::getGravity() const
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
		if (entity->getEntityType() >= physicsComponent)
		{
			//worldTree.insert(entity->getPhysics());
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
		if (entityList[a]->getEntityType() == physicsComponent)
		{
			//worldTree.remove(ID);
		}
		IEntity* e = entityList[a];
		/*for (int a = 0; a < collisions.size(); a++)
		{
			if (collisions[a].manifold.thisMesh->getOwner()->getParent() == e ||
				collisions[a].manifold.otherMesh->getOwner()->getParent() == e)
			{
				collisions.erase(collisions.begin() + a);
				a--;
			}
		}*/
		delete entityList.at(a);
		entityList.erase(entityList.begin() + a);
	}

	void World::traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut)
	{
		/*resultOut.ray = ray;
		resultOut.rayDist = dist;

		vector<IPhysicsComponent*> possibleCollisions;
		worldTree.retrieveCollisions(possibleCollisions, ray, dist);

		for (IPhysicsComponent* collider : possibleCollisions)
		{
			if (collider->getCollisionMesh()->testRay(params, resultOut))
			{
				return;
			}
		}
		resultOut.firstCollision = nullptr;
		resultOut.didHit = false;
		resultOut.collisionDist = -1;*/
	}

	int World::registerMovementState(const std::string & name, const CheckIfMovementState & CheckMovementState, const DoOnMovementState & OnMovementState, const OnMovementStateEnabled& OnStateEnabled, const OnMovementStateDisabled& OnStateDisabled)
	{
		RegisteredMovementState state(name, CheckMovementState, OnMovementState, OnStateEnabled, OnStateDisabled);
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

	void World::updateOctreeIndex(IEntity* entity)
	{
		worldTree.reinsert(entity);
	}

	IWorld::~IWorld() {}
}

