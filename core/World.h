#pragma once
#include "IWorld.h"

namespace ginkgo
{
	class IEntity;

	class World : IWorld
	{
	private:
		float gravity;
		vector<IEntity*>  entityList;
	public:
		virtual void World(float gravity);
		virtual vector<IEntity*> getEntityList() const;

		virtual vector<IEntity*> getEntitiesByType(EntityType type) const override;

		virtual void clearWorld() override;

		virtual void setGravity(float gravity) override;
		virtual void setEntity(int ID) override;

		virtual float getGravity() const override;
		virtual IEntity* getEntity(int ID) const override;

		virtual void addEntity(IEntity* entity) override;
		virtual void removeEntity(IEntity* entity) override;

	};
}