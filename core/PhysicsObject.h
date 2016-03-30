#pragma once

#include "IPhysicsObject.h"

namespace ginkgo
{
	class PhysicsObject : IPhysicsObject
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
		UINT32 collisionFlags;

	public:
		PhysicsObject(ICollisionMesh* collision, float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, bool canGravity = true, bool canCollide = true, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
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
		EntityType getEntityType() const override;

		virtual void checkCollisions() override;
		virtual void resolveCollisions() override;

		virtual void setMaterial(const Material& mat) override;
		virtual void setMass(float mass) override;
		virtual void setCanCollide(bool collides) override;
		virtual void setCanGravity(bool canGravity) override;
		virtual void setCollisionMesh(ICollisionMesh* collision) override;

		virtual const Material& getMaterial() const override;
		virtual float getMass() const override;
		virtual bool doesCollide() const override;
		virtual bool doesHaveGravity() const override;
		virtual UINT32 getCollisionFlags() const override;
		virtual ICollisionMesh* getCollisionMesh() const override;
	};
}