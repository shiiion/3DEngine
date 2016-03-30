#pragma once
#include "IWorld.h"

namespace ginkgo
{
	class IEntity;

	class World : public IWorld
	{
	private:
		float gravity;
		vector<IEntity*>  entityList;
	public:
		World(float gravity);
		virtual vector<IEntity*> getEntitiesByType(EntityType type) const override;
		virtual const vector<IEntity*>& getEntityList() const;

		virtual void clearWorld() override;

		virtual void setGravity(float gravity) override;
		virtual void setEntity(long ID, IEntity* entity) override;

		virtual float getGravity() const override;
		virtual IEntity* getEntity(long ID) const override;

		virtual void addEntity(IEntity* entity) override;
		virtual void removeEntity(long ID) override;

	};
}