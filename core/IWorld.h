#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IEntity;

	class IWorld
	{
	public:
		virtual vector<IEntity*> getEntitiesByType(EntityType type) const = 0;
		virtual vector<IEntity*> getEntityList() const = 0;

		virtual void clearWorld() = 0;

		virtual void setGravity(float gravity) = 0;
		virtual void setEntity(int ID) = 0;

		virtual float getGravity() const = 0;
		virtual IEntity* getEntity(int ID) const = 0;

		virtual void addEntity(IEntity* entity) = 0;
		virtual void removeEntity(IEntity* entity) = 0;

	};
}