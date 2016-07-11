#include "PhysicsObject.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"
#include "ISurface.h"
#include <glm/gtx/projection.hpp>

namespace ginkgo
{
	const float PhysicsObject::minFallAngle = 35;

	PhysicsObject::PhysicsObject(ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity, bool canCollide, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		this->mass = mass;
		material = mat;
		scale = scl;
		this->mesh = mesh;
		collisionMesh = collision;
		position = pos;
		rotation = rot;
		acceleration = accel;
		velocity = vel;
		this->canCollide = canCollide;
		this->canGravity = canGravity;
		collisionState = CSTATE_NOCOLLISION;
		this->collisionType = collisionType;
		entityID = Core::generateID();
		collision->setOwner(this);
	}
	const glm::vec3& PhysicsObject::getScale() const
	{
		return scale;
	}

	void PhysicsObject::setScale(const glm::vec3& scl)
	{
		scale = scl;
	}

	IRenderMesh const* PhysicsObject::getRenderMesh() const
	{
		return mesh;
	}

	void PhysicsObject::setRenderMesh(IRenderMesh const* mesh)
	{
		this->mesh = mesh;
	}

	void PhysicsObject::render()
	{
		//TODO: implement me
	}

	void PhysicsObject::checkCollision(float deltaTime, IPhysicsObject* other)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			return;
		}
		ICollisionMesh* otherCollision = other->getCollisionMesh();

		if (!collisionMesh->testCollision(*otherCollision, deltaTime))
		{
			colliders.push_back(other);
			collisionState = CSTATE_FIRSTCOLLIDE;
		}
		else
		{
			collisionState = CSTATE_NOCOLLISION;
		}
	}

	void PhysicsObject::resolveCollisions(float deltaTime)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			return;
		}
		if (collisionState == CSTATE_FIRSTCOLLIDE)
		{
			CollisionInfo const& generatedInfo = collisionMesh->resolveCollision();
			glm::vec3 A = generatedInfo.collisionNormal, B = collisionMesh->getLastMove().velStart;
			glm::vec3 perp = (glm::dot(A, A) / glm::dot(B, A)) * B - A;
			if (glm::length(perp) == 0)
			{
				velocity = glm::vec3(0, 0, 0);
			}
			else
			{
				velocity = (glm::dot(perp, velocity) / (perp.x * perp.x + perp.y * perp.y + perp.z * perp.z)) * perp;
			}
		}
		position = collisionMesh->getCenter();


		colliders.clear();
		//TODO: resolve this and other (if other is dynamic)
		//TODO: walking: get parallel vector to surface, project velocity onto parallel vector
	}

	bool PhysicsObject::isWalkableNormal(glm::vec3 const& normal)
	{
		if (normal.y <= 0)
		{
			return false;
		}
		glm::vec3 normalized = glm::normalize(normal);
		glm::vec3 vertical(0, -1, 0);

		float angBetween = glm::acos(glm::dot(-normalized, vertical));
		if (angBetween >= minFallAngle)
		{
			return false;
		}
		return true;
	}

	const glm::vec3& PhysicsObject::getAcceleration() const
	{
		return acceleration;
	}

	const glm::vec3& PhysicsObject::getVelocity() const
	{
		return velocity;
	}

	const glm::vec3& PhysicsObject::getRotation() const
	{
		return rotation;
	}

	const glm::vec3& PhysicsObject::getPosition() const
	{
		return position;
	}

	const Material& PhysicsObject::getMaterial() const
	{
		return material;
	}

	float PhysicsObject::getMass() const
	{
		return mass;
	}

	bool PhysicsObject::doesCollide() const
	{
		return canCollide;
	}

	bool PhysicsObject::doesHaveGravity() const
	{
		return canGravity;
	}

	UINT32 PhysicsObject::getCollisionState() const
	{
		return collisionState;
	}

	ICollisionMesh* PhysicsObject::getCollisionMesh() const
	{
		return collisionMesh;
	}

	long PhysicsObject::getEntityID() const
	{
		return entityID;
	}

	UINT32 PhysicsObject::getMovementState() const
	{
		return movementState;
	}

	UINT32 PhysicsObject::getCollisionType() const
	{
		return collisionType;
	}

	bool PhysicsObject::isMoving() const
	{
		return glm::length(velocity) > 0.f || glm::length(acceleration) > 0.0f;
	}

	void PhysicsObject::setAcceleration(const glm::vec3& accel)
	{
		acceleration = accel;
	}

	void PhysicsObject::setVelocity(const glm::vec3& vel)
	{
		velocity = vel;
	}

	void PhysicsObject::setRotation(const glm::vec3& rot)
	{
		rotation = rot;
	}

	void PhysicsObject::setPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	void PhysicsObject::setMaterial(const Material& mat)
	{
		material = mat;
	}

	void PhysicsObject::setMass(float mass)
	{
		this->mass = mass;
	}

	void PhysicsObject::setCanCollide(bool collides)
	{
		canCollide = collides;
	}

	void PhysicsObject::setCanGravity(bool canGravity)
	{
		this->canGravity = canGravity;
	}

	void PhysicsObject::setCollisionMesh(ICollisionMesh* collision)
	{
		collisionMesh = collision;
	}

	void PhysicsObject::setEntityID(long ID)
	{
		entityID = ID;
	}

	void PhysicsObject::setCollisionState(UINT32 state)
	{
		collisionState = state;
	}

	void PhysicsObject::setMovementState(UINT32 state)
	{
		movementState = state;
	}

	EntityType PhysicsObject::getEntityType() const
	{
		return physicsObject;
	}

	bool PhysicsObject::CollisionAlreadyExists(IPhysicsObject* other) const
	{
		bool found = false;
		for (IPhysicsObject* collider : colliders)
		{
			found |= other == collider;
		}
		return found;
	}

	void PhysicsObject::tick(float elapsedTime)
	{
		collisionMesh->generateVertexPath(elapsedTime);
		position += velocity * elapsedTime;
		if (movementState != MSTATE_GROUND)
		{
			velocity.y -= getWorld()->getGravity() * elapsedTime;
		}
		velocity += acceleration * elapsedTime;
	}

	IPhysicsObject* physicsObjectFactory(ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity, bool canCollide, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		return new PhysicsObject(collision, collisionType, mass, mat, mesh, pos, canGravity, canCollide, scl, rot, vel, accel);
	}

}