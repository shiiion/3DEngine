#pragma once

#include "CoreReource.h"


namespace ginkgo
{
	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual glm::vec3 const* getBoundingVertices() const = 0;
		virtual MoveInfo const& getLastMove() const = 0;
		virtual void generateVertexPath(float deltaTime) = 0;

		virtual void setOwner(IPhysicsObject* owner) = 0;

		virtual bool testCollision(ICollisionMesh const& other, float deltaTime, CollisionInfo& collisionOut) = 0;

		virtual float getCollisionTime(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime) const = 0;

		virtual glm::vec3 const& getAxis(int axis) const = 0;
		virtual glm::vec3 const& getCenter() const = 0;
		virtual float getExtent(int extent) const = 0;
		virtual void generateCollisionInfo(ICollisionMesh const& other, CollisionInfo& collisionOut) = 0;
		virtual void resolveCollision(CollisionInfo& manifold) = 0;
		
		virtual IPhysicsObject* getOwner() const = 0;
	};

	DECLSPEC_CORE ICollisionMesh* createCollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis);
}