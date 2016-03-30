#include "World.h"
#include "IEntity.h"

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
		if (type == 1) {
			for (int a = 0; a < entityList.size(); a++)
			{
				newEntityList.at(a) = entityList.at(a);
			}
			return newEntityList;
			
		}
		else if (type == 2) {
			for (int a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType > 1)
					newEntityList.push_back(entityList.at(a));
			}
			return newEntityList;
		}
		else if (type == 3) {
			for (int a = 0; a < entityList.size(); a++)
			{
				if (entityList.at(a)->getEntityType > 2)
					newEntityList.push_back(entityList.at(a));
			}
			return newEntityList;
		}
		else
			return newEntityList;
	}

	void World::clearWorld()
	{

	}

	void World::setGravity(float gravity)
	{
		this->gravity = gravity;
	}
	void World::setEntity(long ID, IEntity* entity)
	{
		oldEntity = World::getEntity(ID);

		World::removeEntity(ID);
		World::addEntity(entity);
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
		entityList.erase(entityList.begin() + a);
	}
}