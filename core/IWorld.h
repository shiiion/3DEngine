#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IEntity;
	class Octree;
	struct Collision;

	class IWorld
	{
	public:
		virtual vector<IEntity*> getEntitiesByType(EntityType type) const = 0;
		virtual const vector<IEntity*>& getEntityList() const = 0;

		virtual void clearWorld() = 0;

		virtual void setGravity(float gravity) = 0;
		//virtual void setEntity(long ID, IEntity* entity) = 0;

		virtual glm::vec3 const& getGravity() const = 0;
		virtual IEntity* getEntity(long ID) const = 0;

		virtual void addEntity(IEntity* entity) = 0;
		virtual void removeEntity(long ID) = 0;

		virtual void traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut) = 0;

		virtual Octree const& getEntityTree() const = 0;

		virtual CustomMovement* getCustomMovement(int movementValue) const = 0;

		virtual void registerCustomMovement(CustomMovement const& newMove) = 0;

		virtual void addCollision(CollisionInfo const& info, float deltaTime) = 0;
		virtual void clearCollisionCache() = 0;

		virtual void resolveCollisions(INT32 iterations) = 0;

		virtual bool collisionExists(IPhysicsObject* a, IPhysicsObject* b) const = 0;
	};
}