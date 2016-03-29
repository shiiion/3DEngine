#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IEntity;

	class IWorld
	{
	public:
		virtual vector<IEntity*> getEntitiesByType(EntityType type) const = 0;

		virtual void clearWorld() = 0;

		virtual float getGravity() const = 0;
		virtual void setGravity(float gravity) = 0;
		virtual IEntity* getEntity(int id) const = 0;
		virtual void setEntity(int id) const = 0;

		virtual void addEntity(const IEntity* entity) const = 0;
		virtual void removeEntity(const IEntity* entity) const = 0;

	};
}