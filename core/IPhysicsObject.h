#pragma once

#include "IRenderable.h"

#define CSTATE_REBOUND 1
#define CSTATE_COLLIDING 2
#define CSTATE_FIRSTCOLLIDE 3
#define CSTATE_NOCOLLISION 0

#define MSTATE_AIR 1
#define MSTATE_GROUND 2
//MORE STUFF CAN BE ADDED

//object is never resolved (pushes objects out of the way)
//typically for objects which will never move, such as floor, wall, etc
#define CTYPE_WORLDSTATIC 1
//object is always resolved (pushed out of the way)
//typically for moving objects in the world
#define CTYPE_WORLDDYNAMIC 2
//object is always resolved (pushed out of the way)
//typically for the player object, as it doesn't bounce off of surfaces
#define CTYPE_WORLDDYNAMIC_NOREBOUND 3

#define VERTEX_FACE_COLLISION 1
#define FACE_VERTEX_COLLISION 2
#define FACE_EDGE_COLLISION 3
#define EDGE_FACE_COLLISION 4
#define FACE_FACE_COLLISION 5
#define EDGE_EDGE_COLLISION 6
#define VERTEX_VERTEX_COLLISION 7
#define EDGE_VERTEX_COLLISION 8
#define VERTEX_EDGE_COLLISION 9

namespace ginkgo
{
	class ICollisionMesh;

	class IPhysicsObject : public IRenderable
	{
	public:
		virtual void checkCollision(float deltaTime, IPhysicsObject* other) = 0;
		virtual void resolveCollisions(float deltaTime) = 0;
		
		virtual void setMaterial(const Material& mat) = 0;
		virtual void setMass(float mass) = 0;
		virtual void setCanCollide(bool collides) = 0;
		virtual void setCanGravity(bool canGravity) = 0;
		virtual void setCollisionMesh(ICollisionMesh* collision) = 0;
		virtual void setCollisionState(UINT32 state) = 0;
		virtual void setMovementState(UINT32 state) = 0;
		
		virtual const Material& getMaterial() const = 0;
		virtual float getMass() const = 0;
		virtual bool doesCollide() const = 0;
		virtual bool doesHaveGravity() const = 0;
		virtual UINT32 getCollisionState() const = 0;
		virtual ICollisionMesh* getCollisionMesh() const = 0;
		virtual UINT32 getMovementState() const = 0;
		virtual UINT32 getCollisionType() const = 0;

		virtual bool isMoving() const = 0;
	};

	DECLSPEC_CORE IPhysicsObject* physicsObjectFactory(ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity = true, bool canCollide = true, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
}