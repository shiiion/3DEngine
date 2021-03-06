#pragma once
#include "IWorld.h"
#include "Octree.h"
#include "Collision.h"
#include "MovementStateCallbackManager.h"
//world space is a box spanning -4000000 ~ 4000000 L, W, and H
#define WORLD_DIMENSIONS -4000000.f, -4000000.f, -4000000.f, 8000000.f, 8000000.f, 8000000.f

namespace ginkgo
{
	class IEntity;
	class Octree;

	class World : public IWorld
	{
	private:
		vec3 gravity;
		vector<IEntity*> entityList;
		Octree worldTree;
		vector<Collision> collisions;
		MovementStateCallbackManager manager;

	public:
		World(float gravity);
		vector<IEntity*> getEntitiesByType(EntityType type) const override;
		const vector<IEntity*>& getEntityList() const;

		void clearWorld() override;

		void setGravity(float gravity) override;
		//virtual void setEntity(long ID, IEntity* entity) override;

		vec3 const& getGravity() const override;
		IEntity* getEntity(long ID) const override;

		void addEntity(IEntity* entity) override;
		void removeEntity(long ID) override;

		void traceRayThroughWorld(Ray const& ray, float dist, RaytraceParams& params, RaytraceResult& resultOut) override;

		//void registerCustomMovement(CustomMovement const& newMove) override;
		//CustomMovement* getCustomMovement(int movementValue) const override;

		virtual int registerMovementState(const std::string& name, const CheckIfMovementState& CheckMovementState, const DoOnMovementState& OnMovementState, const OnMovementStateEnabled& OnStateEnabled, const OnMovementStateDisabled& OnStateDisabled) override;
		virtual int getMovementState(const std::string& name) const override;
		virtual void checkMovementStates(float elapsedTime) override;
		virtual void doMovementStates(float elapsedTime) override;

		Octree const& getEntityTree() const override
		{
			return worldTree;
		}

		void addCollision(CollisionInfo const& info, float deltaTime) override;
		void clearCollisionCache() override;

		void resolveCollisions(INT32 iterations) override;
		bool collisionExists(IPhysicsObject* a, IPhysicsObject* b) const override;

		void preCollisionTest();
		void updateOctreeIndex(IEntity* entity);
	};
}