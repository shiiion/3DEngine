#include "PhysicsObject.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"
#include "ISurface.h"
#include "Collision.h"

namespace ginkgo
{

	PhysicsObject::PhysicsObject(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide)
		: parent(parent)
	{
		this->mass = mass;
		material = mat;
		collisionMesh = collision;
		this->canCollide = canCollide;
		numCollisions = 0;
		this->collisionType = collisionType;
		collision->setOwner(this);
		collision->setRotation(parent->getRotation());
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
			normalList.emplace_front(SurfaceData(parent->getEntityID(), other->getParent()->getEntityID(), manifold.collisionNormal));
			((PhysicsObject*)other)->normalList.emplace_front(SurfaceData(other->getParent()->getEntityID(),
				parent->getEntityID(), -manifold.collisionNormal));
			return true;
		}
		return false;
	}

	void PhysicsObject::setFinalMove(MoveResult const& result)
	{    
		finalMove = result;
	}

	const PhysMaterial& PhysicsObject::getMaterial() const
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

	std::forward_list<SurfaceData> const& PhysicsObject::getCollisionNormalList() const
	{
		return normalList;
	}

	void PhysicsObject::removeNormal(SurfaceData const& data)
	{
		for (auto iter = normalList.before_begin(); iter != normalList.end();)
		{
			auto prevElem = iter;
			iter++;
			if ((iter)->compareSurface(data))
			{
				normalList.erase_after(prevElem);
				break;
			}
		}
	}

	void PhysicsObject::setMaterial(const PhysMaterial& mat)
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

	void PhysicsObject::setRotation(const quat& rotation)
	{
		rotationBuffer = rotation;
		if (collisionMesh != nullptr)
		{
			collisionMesh->setRotation(rotation);
		}
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
	
	IPhysicsObject* physicsObjectFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide)
	{
		return new PhysicsObject(parent, collision, collisionType, mass, mat, canCollide);
	}

	IPhysicsObject::~IPhysicsObject() {}
}