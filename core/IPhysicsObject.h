#pragma once

#include "IRenderable.h"

#define CSTATE_REBOUND 1
#define CSTATE_COLLIDING 2
#define CSTATE_FIRSTCOLLIDE 3
#define CSTATE_NOCOLLISION 0

namespace ginkgo
{
	class ICollisionMesh;

	class IPhysicsObject : public IRenderable
	{
	public:
		virtual void checkCollisions() = 0;
		virtual void resolveCollisions() = 0;
		
		virtual void setMaterial(const Material& mat) = 0;
		virtual void setMass(float mass) = 0;
		virtual void setCanCollide(bool collides) = 0;
		virtual void setCanGravity(bool canGravity) = 0;
		virtual void setCollisionMesh(ICollisionMesh* collision) = 0;
		
		virtual const Material& getMaterial() const = 0;
		virtual float getMass() const = 0;
		virtual bool doesCollide() const = 0;
		virtual bool doesHaveGravity() const = 0;
		virtual UINT32 getCollisionState() const = 0;
		virtual ICollisionMesh* getCollisionMesh() const = 0;
	};

	DECLSPEC IPhysicsObject* physicsObjectFactory(ICollisionMesh* collision, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity = true, bool canCollide = true, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
}