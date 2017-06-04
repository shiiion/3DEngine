#pragma once

#include "CoreReource.h"


namespace ginkgo
{

#define CMESH_SHAPE_OBB 1
#define CMESH_SHAPE_CAPSULE 2

	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual MoveInfo const& getLastMove() const = 0;
		virtual void generateVertexPath(float deltaTime) = 0;

		virtual void setOwner(IPhysicsObject* owner) = 0;

		virtual bool testCollision(ICollisionMesh const& other, float deltaTime, CollisionInfo& collisionOut) = 0;
		//collisionOut should provide an axis normal to get penetration distance
		virtual bool testCollisionStationary(ICollisionMesh const& other, CollisionStationary& collisionOut) = 0;
		virtual bool testRay(RaytraceParams& params, RaytraceResult& resultOut) const = 0;

		virtual void generateCollisionInfo(ICollisionMesh const& other, CollisionInfo& collisionOut) = 0;
		virtual float getAxisOverlap(vec3 const& axisNorm, ICollisionMesh const& other) const = 0;

		virtual IPhysicsObject* getOwner() const = 0;

		virtual void setCachedCenter(vec3 const& center) = 0;
		virtual vec3 const& getCachedCenter() const = 0;

		virtual void setCachedVelocity(vec3 const& vel) = 0;
		virtual vec3 const& getCachedVelocity() const = 0;

		virtual int getCollisionShape() const = 0;
	};

	DECLSPEC_CORE ICollisionMesh* createCollisionMesh(float w, float h, float l, vec3 const& wAxis, vec3 const& hAxis, vec3 const& lAxis);
}