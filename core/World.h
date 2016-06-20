#pragma once
#include "IWorld.h"
#include "Octree.h"
//world space is a box spanning -4000000 ~ 4000000 L, W, and H
#define WORLD_DIMENSIONS -4000000.f, -4000000.f, -4000000.f, 8000000.f, 8000000.f, 8000000.f

namespace ginkgo
{
	class IEntity;

	class World : public IWorld
	{
	private:
		float gravity;
		vector<IEntity*> entityList;
		Octree worldTree;

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

		virtual void traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut) override;

	};
}