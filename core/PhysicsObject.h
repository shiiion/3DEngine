#pragma once

#include "IPhysicsObject.h"

namespace ginkgo
{
	class PhysicsObject : public IPhysicsObject
	{
	private:
		glm::vec3 scale;
		IRenderMesh const* mesh;
		
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		
		long entityID;

		ICollisionMesh* collisionMesh;
		Material material;
		float mass;
		bool canCollide;
		bool canGravity;
		std::vector<IPhysicsObject*> colliders;
		UINT32 collisionState;
		UINT32 movementState;
		UINT32 collisionType;

		//FOR WALKING
		IPhysicsObject* walkSurface;
		glm::vec3 surfaceNormal;
		//minimum angle between normal and vertical (0, -1, 0) at which falling turns to walking
		static const float minFallAngle;

		bool isWalkableNormal(glm::vec3 const& normal);

	public:
		PhysicsObject(ICollisionMesh* collision, UINT32 collisionType, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity = true, bool canCollide = true, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
		virtual const glm::vec3& getScale() const override;
		virtual void setScale(const glm::vec3& scl) override;
		virtual IRenderMesh const* getRenderMesh() const override;
		virtual void setRenderMesh(IRenderMesh const* mesh) override;
		virtual void render() override;

		virtual void tick(float elapsedTime) override;

		virtual const glm::vec3& getPosition() const override;
		virtual const glm::vec3& getVelocity() const override;
		virtual const glm::vec3& getAcceleration() const override;
		virtual const glm::vec3& getRotation() const override;
		virtual long getEntityID() const override;

		virtual void setPosition(const glm::vec3& pos) override;
		virtual void setVelocity(const glm::vec3& vel) override;
		virtual void setAcceleration(const glm::vec3& acc) override;
		virtual void setRotation(const glm::vec3& ang) override;
		virtual void setEntityID(long ID) override;
		EntityType getEntityType() const override;

		virtual void checkCollision(float deltaTime, IPhysicsObject* other) override;
		virtual void resolveCollisions(float deltaTime) override;

		virtual void setMaterial(const Material& mat) override;
		virtual void setMass(float mass) override;
		virtual void setCanCollide(bool collides) override;
		virtual void setCanGravity(bool canGravity) override;
		virtual void setCollisionMesh(ICollisionMesh* collision) override;
		virtual void setCollisionState(UINT32 state) override;
		virtual void setMovementState(UINT32 state) override;

		virtual const Material& getMaterial() const override;
		virtual float getMass() const override;
		virtual bool doesCollide() const override;
		virtual bool doesHaveGravity() const override;
		virtual UINT32 getCollisionState() const override;
		virtual ICollisionMesh* getCollisionMesh() const override;
		virtual UINT32 getMovementState() const override;
		virtual UINT32 getCollisionType() const override;

		virtual bool CollisionAlreadyExists(IPhysicsObject* other) const override;

		virtual bool isMoving() const override;
	};
}