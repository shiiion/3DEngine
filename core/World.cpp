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
				IPhysicsObject* t = collisions[a].manifold.thisMesh->getOwner();
				IPhysicsObject* o = collisions[a].manifold.otherMesh->getOwner();
				t->decrementCollision();
				o->decrementCollision();
				//TODO: give IDs to collisions for better stability
				t->removeNormal(SurfaceData(t->getParent()->getEntityID(), o->getParent()->getEntityID(), collisions[a].manifold.normal));
				o->removeNormal(SurfaceData(o->getParent()->getEntityID(), t->getParent()->getEntityID(), -collisions[a].manifold.normal));
				collisions.erase(collisions.begin() + a);
				a--;
			}
		}
	}

	void World::resolveCollisions(INT32 iterations)
	{
		//vec3 pos, pos2;
		//vec3 v, v2; float t;
		//for (Collision& c : collisions)
		//{
		//	if (c.overlapSkipFixRan || !c.manifold.thisMesh->testCollisionStationary(*c.manifold.otherMesh, c.manifold))
		//	{
		//		c.overlapSkipFixRan = true;
		//		continue;
		//	}
		//	c.overlapSkipFixRan = true;
		//	pos = c.manifold.thisMesh->getCachedCenter();
		//	v = c.manifold.thisMesh->getCachedVelocity();
		//	pos2 = c.manifold.otherMesh->getCachedCenter();
		//	v2 = c.manifold.otherMesh->getCachedVelocity();

		//	float magn = glm::length(v), magn2 = glm::length(v2);

		//	if (magn <= 0.000001 && magn2 <= 0.000001)
		//	{
		//		continue;
		//	}
		//	
		//	t = magn2 == 0 || magn > magn2 ? (-v.z * pos.z - v.y * pos.y - v.x * pos.x + v.x * pos2.x + v.y * pos2.y + v.z * pos2.z) / (glm::pow(v.x, 2) + glm::pow(v.y, 2) + glm::pow(v.z, 2))
		//		: (-v2.z * pos2.z - v2.y * pos2.y - v2.x * pos2.x + v2.x * pos.x + v2.y * pos.y + v2.z * pos.z) / (glm::pow(v2.x, 2) + glm::pow(v2.y, 2) + glm::pow(v2.z, 2));
		//	if (magn == 0 || magn > magn2)
		//		c.manifold.thisMesh->setCachedCenter(vec3(pos.x + v.x * t, pos.y + v.y * t, pos.z + v.z * t));
		//	else
		//		c.manifold.otherMesh->setCachedCenter(vec3(pos2.x + v2.x * t, pos2.y + v2.y * t, pos2.z + v2.z * t));
		//	//equation: t = (-vz * z0 - vy * y0 - vx * x0 + vx * x1 + vy * y1 + vz * z1) / (vx^2 + vy^2 + vz^2)
		//	//solve for t where x1y1z1 is the still object and x0y0z0 is the moving object
		//	//if both are moving then make x1y1z1 be the slower moving object of the two objects and pick x1y1z1 as the initial position
		//	//set position of x0y0z0 to be x0 + vx * t, y0 + vy * t, z0 + vz * t
		//}
		if (collisions.size() == 0)
		{
			return;
		}

		for (INT32 a = 0; a < iterations; a++)
		{
			for (Collision& c : collisions)
			{
				//if (c.markedForDestruction) continue;
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
			//if (c.markedForDestruction)
			//{
			//	c.valid = false;
			//	continue;
			//}
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
			c.preCollisionUpdate();
		}
	}

	void World::updateOctreeIndex(IEntity* entity)
	{
		worldTree.reinsert(entity);
	}
}

