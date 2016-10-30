#include "PhysicsObject.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"
#include "ISurface.h"
#include "Collision.h"

namespace ginkgo
{

	PhysicsObject::PhysicsObject(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, bool canGravity, bool canCollide)
		: parent(parent)
	{
		this->mass = mass;
		material = mat;
		collisionMesh = collision;
		this->canCollide = canCollide;
		this->canGravity = canGravity;
		if (canGravity && collisionType == CTYPE_WORLDDYNAMIC)
		{
			parent->addAcceleration(getWorld()->getGravity());
		}
		numCollisions = 0;
		this->collisionType = collisionType;
		collision->setOwner(this);
	}

	bool PhysicsObject::checkCollision(float deltaTime, IPhysicsObject* other)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
		{
			return false;
		}
		ICollisionMesh* otherCollision = other->getCollisionMesh();

		CollisionInfo manifold(collisionMesh, other->getCollisionMesh());

		if (!collisionMesh->testCollision(*otherCollision, deltaTime, manifold))
		{
			getWorld()->addCollision(manifold, deltaTime);
			primaryCollisionNormal = manifold.collisionNormal;
			((PhysicsObject*)other)->primaryCollisionNormal = -manifold.collisionNormal;
			return true;
		}
		return false;
	}

	void PhysicsObject::setFinalMove(MoveResult const& result)
	{    
		finalMove = result;
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

	ICollisionMesh* PhysicsObject::getCollisionMesh() const
	{
		return collisionMesh;
	}

	UINT32 PhysicsObject::getNumCollisions() const
	{
		return numCollisions;
	}

	UINT32 PhysicsObject::getCollisionType() const
	{
		return collisionType;
	}

	bool PhysicsObject::isMoving() const
	{
		return glm::length(parent->getVelocity()) > 0.f || glm::length(parent->getVelocity()) > 0.0f;
	}

	glm::vec3 const& PhysicsObject::getPrimaryCollisionNormal() const
	{
		return primaryCollisionNormal;
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

	void PhysicsObject::incrementCollision()
	{
		numCollisions++;
	}

	void PhysicsObject::decrementCollision()
	{
		if (numCollisions > 0)
		{
			numCollisions--;
		}
	}

	void PhysicsObject::setMovementState(UINT32 state)
	{
		movementState = state;
	}

	void PhysicsObject::onTick(float elapsedTime)
	{
		collisionMesh->generateVertexPath(elapsedTime);
		finalMove = MoveResult(collisionMesh->getLastMove().centerEnd, collisionMesh->getLastMove().velEnd);
	}

	IEntity* const PhysicsObject::getParent() const
	{
		return parent;
	}

	const MoveResult& PhysicsObject::getMoveResult() const
	{
		return finalMove;
	}
	
	IPhysicsObject* physicsObjectFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, bool canGravity, bool canCollide)
	{
		return new PhysicsObject(parent, collision, collisionType, mass, mat, canGravity, canCollide);
	}

}