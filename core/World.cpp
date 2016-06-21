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
		this->gravity = gravity;
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
		else if (type == renderable)
		{
			for (UINT32 a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType() > entity)
					newEntityList.push_back(entityList.at(a));
			}
			return newEntityList;
		}
		else if (type == physicsObject)
		{
			for (UINT32 a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType() > renderable)
					newEntityList.push_back(entityList.at(a));
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
		this->gravity = gravity;
	}

	void World::setEntity(long ID, IEntity* entity)
	{
		//TODO: tree stuff here
		IEntity* oldEntity = World::getEntity(ID);
		if (oldEntity == nullptr)
			return;
		IEntity* newEntity;
		if (entity->getEntityType() >= 1 && oldEntity->getEntityType() >= 1)
		{
			entity->setPosition(oldEntity->getPosition());
			entity->setVelocity(oldEntity->getVelocity());
			entity->setAcceleration(oldEntity->getAcceleration());
			entity->setRotation(oldEntity->getRotation());
			entity->setEntityID(oldEntity->getEntityID());
			newEntity = entity;

			if (entity->getEntityType() >= 2 && oldEntity->getEntityType() >= 2)
			{
				IRenderable* renderable = (IRenderable*)entity;
				IRenderable* oldRenderable = (IRenderable*)oldEntity;
				renderable->setRenderMesh(oldRenderable->getRenderMesh());
				renderable->setScale(oldRenderable->getScale());
				newEntity = renderable;

				if (entity->getEntityType() >= 3 && oldEntity->getEntityType() >= 3)
				{
					IPhysicsObject* physicsObject = (IPhysicsObject*)renderable;
					IPhysicsObject* oldPhysicsObject = (IPhysicsObject*)oldRenderable;
					physicsObject->setCollisionMesh(oldPhysicsObject->getCollisionMesh());
					physicsObject->setMass(oldPhysicsObject->getMass());
					physicsObject->setMaterial(oldPhysicsObject->getMaterial());
					physicsObject->setCanGravity(oldPhysicsObject->doesHaveGravity());
					physicsObject->setCanCollide(oldPhysicsObject->doesCollide());
					newEntity = physicsObject;

				}
			}
		}
		else
			return;

		removeEntity(ID);
		addEntity(newEntity);
		//add and remove
	}

	float World::getGravity() const
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
		if (entity->getEntityType() == physicsObject)
		{
			worldTree.insert((IPhysicsObject*)entity);
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
			worldTree.remove((IPhysicsObject*)entityList[a]);
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
			SurfaceCollisionMesh surfaceMesh(collider->getCollisionMesh());
			for (int a = 0; a < 6; a++)
			{
				if (surfaceMesh.faces[a].intersectsWithSurface(ray, dist))
				{
					if (params.func != nullptr)
					{
						if (params.func(collider))
						{
							resultOut.didHit = true;
							resultOut.firstCollision = collider;
							resultOut.collisionDist = surfaceMesh.faces[a].getIntersectionValue(ray);
							return;
						}
					}
					else
					{
						resultOut.didHit = true;
						resultOut.firstCollision = collider;
						resultOut.collisionDist = surfaceMesh.faces[a].getIntersectionValue(ray);
						return;
					}
				}
			}
		}
		resultOut.firstCollision = nullptr;
		resultOut.didHit = false;
		resultOut.collisionDist = -1;
		return;
	}


	void World::registerCustomMovement(CustomMovement const& newMove)
	{
		customMovements.push_back(newMove);
	}


	CustomMovement* World::getCustomMovement(int movementValue) const
	{
		for (int a = 0; a < customMovements.size(); a++)
		{
			if (customMovements[a].movementValue == movementValue)
			{
				return &customMovements[a];
			}
		}
		return nullptr;
	}
}

