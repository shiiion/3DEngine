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

		CollisionInfo manifold(collisionMesh, other->getCollisionMesh());

		if (!collisionMesh->testCollision(*otherCollision, deltaTime, manifold))
		{
			collisions.emplace_back(manifold);
			collisionState = CSTATE_RESOLVE;
		}
		else
		{
			collisionState = CSTATE_NOCOLLISION;
		}
	}

	void PhysicsObject::resolveCollision(CollisionInfo& manifold)
	{
		IPhysicsObject* otherParent = manifold.otherMesh->getOwner();

		//INITIAL COLLISION RESOLUTION: move each mesh back in time
		manifold.thisMesh->resolveCollision(manifold);
		manifold.otherMesh->resolveCollision(manifold);

		//Contact velocity
		float contactVel = glm::dot(otherParent->getVelocity() - velocity, manifold.collisionNormal);

		if (contactVel < 0)
		{
			return;
		}

		//select minimum restitution
		float restitution = glm::min(otherParent->getMaterial().reboundFraction, material.reboundFraction);

		//impulse scalar
		//if type is worldstatic, mass is infinite (1/mass = 0)
		float IS = -(1.f + restitution) * contactVel;
		float invMassThis, invMassOther;
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			invMassThis = 0;
		}
		else
		{
			invMassThis = 1 / mass;
		}
		if (otherParent->getCollisionType() == CTYPE_WORLDSTATIC)
		{
			invMassOther = 0;
		}
		else
		{
			invMassOther = 1 / otherParent->getMass();
		}

		IS /= (invMassThis + invMassOther);

		if (glm::abs(IS) < /*MINREBOUND*/0.1)
		{
			IS = (-contactVel) / (invMassThis + invMassOther);
		}

		glm::vec3 impulse = manifold.collisionNormal * IS;
		velocity -= invMassThis * impulse;
		otherParent->setVelocity(otherParent->getVelocity() + (invMassOther * impulse));
	}

	void PhysicsObject::resolveCollisions(float deltaTime)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			return;
		}
		if (collisionState == CSTATE_RESOLVE)
		{
			for (CollisionInfo& manifold : collisions)
			{
				resolveCollision(manifold);
			}
			finalizeMove(deltaTime, true);
		}
		else
		{
			finalizeMove(0, false);
		}

		collisions.clear();
		//TODO: resolve this and other (if other is dynamic)
		//TODO: walking: get parallel vector to surface, project velocity onto parallel vector
	}

	void PhysicsObject::finalizeMove(float deltaTime, bool resolver)
	{
		position = collisionMesh->getCenter();
		if (resolver)
		{
			float longestResim = 0;
			for (CollisionInfo& manifold : collisions)
			{
				if (deltaTime - manifold.collisionTime > longestResim)
				{
					longestResim = deltaTime - manifold.collisionTime;
				}
				manifold.otherMesh->getOwner()->finalizeMove((deltaTime - manifold.collisionTime), false);
			}
			tick(longestResim);
		}
		else
		{
			tick(deltaTime);
		}
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
		for (CollisionInfo const& manifold : collisions)
		{
			found |= other == manifold.otherMesh->getOwner();
		}
		return found;
	}

	void PhysicsObject::tick(float elapsedTime)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			return;
		}
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