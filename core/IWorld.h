#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IEntity;

	class IWorld
	{
	public:
		virtual vector<IEntity*> getEntitiesByType(EntityType type) const = 0;
		virtual const vector<IEntity*>& getEntityList() const = 0;

		virtual void clearWorld() = 0;

		virtual void setGravity(float gravity) = 0;
		virtual void setEntity(long ID, IEntity* entity) = 0;

		virtual float getGravity() const = 0;
		virtual IEntity* getEntity(long ID) const = 0;

		virtual void addEntity(IEntity* entity) = 0;
		virtual void removeEntity(long ID) = 0;

		virtual void traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut) = 0;

	};
}