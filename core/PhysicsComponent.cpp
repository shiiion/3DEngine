#include "PhysicsComponent.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"
#include "ISurface.h"
#include "Collision.h"

namespace ginkgo
{

	PhysicsComponent::PhysicsComponent(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide)
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

	bool PhysicsComponent::checkCollision(float deltaTime, IPhysicsComponent* other)
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
			((PhysicsComponent*)other)->normalList.emplace_front(SurfaceData(other->getParent()->getEntityID(),
				parent->getEntityID(), -manifold.collisionNormal));
			return true;
		}
		return false;
	}

	void PhysicsComponent::setFinalMove(MoveResult const& result)
	{    
		finalMove = result;
	}

	const PhysMaterial& PhysicsComponent::getMaterial() const
	{
		return material;
	}

	float PhysicsComponent::getMass() const
	{
		return mass;
	}

	bool PhysicsComponent::doesCollide() const
	{
		return canCollide;
	}

	ICollisionMesh* PhysicsComponent::getCollisionMesh() const
	{
		return collisionMesh;
	}

	UINT32 PhysicsComponent::getNumCollisions() const
	{
		return numCollisions;
	}

	UINT32 PhysicsComponent::getCollisionType() const
	{
		return collisionType;
	}

	bool PhysicsComponent::isMoving() const
	{
		return glm::length(parent->getVelocity()) > 0.f || glm::length(parent->getVelocity()) > 0.0f;
	}

	std::forward_list<SurfaceData> const& PhysicsComponent::getCollisionNormalList() const
	{
		return normalList;
	}

	void PhysicsComponent::removeNormal(SurfaceData const& data)
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

	void PhysicsComponent::setMaterial(const PhysMaterial& mat)
	{
		material = mat;
	}

	void PhysicsComponent::setMass(float mass)
	{
		this->mass = mass;
	}

	void PhysicsComponent::setCanCollide(bool collides)
	{
		canCollide = collides;
	}

	void PhysicsComponent::setCollisionMesh(ICollisionMesh* collision)
	{
		collisionMesh = collision;
	}

	void PhysicsComponent::incrementCollision()
	{
		numCollisions++;
	}

	void PhysicsComponent::decrementCollision()
	{
		if (numCollisions > 0)
		{
			numCollisions--;
		}
	}

	void PhysicsComponent::setMovementState(UINT32 state)
	{
		movementState = state;
	}

	void PhysicsComponent::setRotation(const quat& rotation)
	{
		rotationBuffer = rotation;
		if (collisionMesh != nullptr)
		{
			collisionMesh->setRotation(rotation);
		}
	}

	void PhysicsComponent::onTick(float elapsedTime)
	{
		collisionMesh->generateVertexPath(elapsedTime);
		finalMove = MoveResult(collisionMesh->getLastMove().centerEnd, collisionMesh->getLastMove().velEnd);
	}

	IEntity* const PhysicsComponent::getParent() const
	{
		return parent;
	}

	const MoveResult& PhysicsComponent::getMoveResult() const
	{
		return finalMove;
	}
	
	IPhysicsComponent* PhysicsComponentFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat, bool canCollide)
	{
		return new PhysicsComponent(parent, collision, collisionType, mass, mat, canCollide);
	}

	IPhysicsComponent::~IPhysicsComponent() {}

	IComponent::~IComponent() {}
}