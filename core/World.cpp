#include "World.h"
#include "IEntity.h"
#include "IRenderable.h"
#include "IPhysicsObject.h"

namespace ginkgo
{

	World::World(float gravity)
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
			for (int a = 0; a < entityList.size(); a++)
			{
				newEntityList.at(a) = entityList.at(a);
			}
			return newEntityList;

		}
		else if (type == renderable)
		{
			for (int a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType() > entity)
					newEntityList.push_back(entityList.at(a));
			}
			return newEntityList;
		}
		else if (type == physicsObject)
		{
			for (int a = 0; a < entityList.size(); a++)
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
		for (int a = 0; a < entityList.size(); a++)
		{
			delete entityList.at(a);
		}
		entityList.clear();
	}

	void World::setGravity(float gravity)
	{
		this->gravity = gravity;
	}

	void World::setEntity(long ID, IEntity* entity)
	{
		IEntity* oldEntity = World::getEntity(ID);
		IEntity* newEntity;
		if (entity->getEntityType() >= 1 && oldEntity->getEntityType() >= 1) 
		{
			entity->setPosition(oldEntity->getPosition);
			entity->setVelocity(oldEntity->getVelocity);
			entity->setAcceleration(oldEntity->getAcceleration);
			entity->setRotation(oldEntity->getRotation);
			entity->setEntityID(oldEntity->getEntityID);
			newEntity = entity;

			if (entity->getEntityType() >= 2 && oldEntity->getEntityType() >= 2) 
			{
				IRenderable* renderable = (IRenderable*)entity;
				IRenderable* oldRenderable = (IRenderable*)oldEntity;
				renderable->setRenderMesh(oldRenderable->getRenderMesh);
				renderable->setScale(oldRenderable->getScale);
				newEntity = renderable;

				if (entity->getEntityType() >= 3 && oldEntity->getEntityType() >= 3)
				{
					IPhysicsObject* physicsObject = (IPhysicsObject*)renderable;
					IPhysicsObject* oldPhysicsObject = (IPhysicsObject*)oldRenderable;
					physicsObject->setCollisionMesh(oldPhysicsObject->getCollisionMesh);
					physicsObject->setMass(oldPhysicsObject->getMass);
					physicsObject->setMaterial(oldPhysicsObject->getMaterial);
					physicsObject->setCanGravity(oldPhysicsObject->doesHaveGravity);
					physicsObject->setCanCollide(oldPhysicsObject->doesCollide);
					newEntity = physicsObject;

				}
			}
		}

		World::removeEntity(ID);
		World::addEntity(newEntity);
		//add and remove
	}

	float World::getGravity() const
	{
		return gravity;
	}

	IEntity* World::getEntity(long ID) const
	{
		int a;
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
	}

	void World::removeEntity(long ID)
	{
		int a;
		for (a = 0; a < entityList.size(); a++)
		{
			if (entityList.at(a)->getEntityID() == ID)
				break;
		}
		if (a == entityList.size())
			return;
		delete entityList.at(a);
		entityList.erase(entityList.begin() + a);
	}
}