#include "PhysicsObject.h"
#include "Core.h"
#include "IWorld.h"
#include "ICollisionMesh.h"

namespace ginkgo
{
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

	void PhysicsObject::checkCollisions(float deltaTime)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
			return;
	}

	void PhysicsObject::resolveCollisions(float deltaTime)
	{
		if (collisionType == CTYPE_WORLDSTATIC)
			return;
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

	EntityType PhysicsObject::getEntityType() const
	{
		return physicsObject;
	}

	UINT32 PhysicsObject::getMovementState() const
	{
		return movementState;
	}

	UINT32 PhysicsObject::getCollisionType() const
	{
		return collisionType;
	}

	void PhysicsObject::tick(float elapsedTime)
	{
		collisionMesh->generateVertexPath(elapsedTime, this);
		velocity.y -= getWorld()->getGravity() * elapsedTime;
		velocity += acceleration * elapsedTime;
		position += velocity * elapsedTime;
	}

	IPhysicsObject* physicsObjectFactory(ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity, bool canCollide, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		return new PhysicsObject(collision, collisionType, mass, mat, mesh, pos, canGravity, canCollide, scl, rot, vel, accel);
	}

}