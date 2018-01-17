#include "PhysicsComponent.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"
#include "ISurface.h"

namespace ginkgo
{

	PhysicsComponent::PhysicsComponent(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat)
		: parent(parent)
	{
		this->mass = mass;
		material = mat;
		collisionMesh = collision;
		this->collisionType = collisionType;
		collision->setOwner(this);
	}

	const PhysMaterial& PhysicsComponent::getMaterial() const
	{
		return material;
	}

	float PhysicsComponent::getMass() const
	{
		return mass;
	}

	ICollisionMesh* PhysicsComponent::getCollisionMesh() const
	{
		return collisionMesh;
	}

	UINT32 PhysicsComponent::getCollisionType() const
	{
		return collisionType;
	}

	void PhysicsComponent::setMaterial(const PhysMaterial& mat)
	{
		material = mat;
	}

	void PhysicsComponent::setMass(float mass)
	{
		this->mass = mass;
	}

	void PhysicsComponent::setCollisionMesh(ICollisionMesh* collision)
	{
		collisionMesh = collision;
	}

	void PhysicsComponent::setMovementState(UINT32 state)
	{
		movementState = state;
	}

	void PhysicsComponent::onTick(float elapsedTime)
	{
	}

	void PhysicsComponent::onTickEnd(float elapsedTime)
	{

	}

	int PhysicsComponent::type() const
	{
		return PHYSICS_COMPONENT;
	}
	
	void PhysicsComponent::onDetach()
	{
		// Remove me from collision stuff
	}

	IEntity* PhysicsComponent::getParent()
	{
		return parent;
	}

	IPhysicsComponent* PhysicsComponentFactory(IEntity* parent, ICollisionMesh* collision, UINT32 collisionType, float mass, PhysMaterial mat)
	{
		return new PhysicsComponent(parent, collision, collisionType, mass, mat);
	}

	IPhysicsComponent::~IPhysicsComponent() {}

	IComponent::~IComponent() {}
}