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

		ICollisionMesh* collisionMesh;
		Material material;
		float mass;
		bool canCollide;
		bool canGravity;
		UBYTE collisionFlags;

	public:
		PhysicsObject(float mass, Material mat, IRenderMesh const* mesh, const glm::vec3& pos, UBYTE collisionFlags=0, bool canGravity = true, bool canCollide = true, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
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

		virtual void setPosition(const glm::vec3& pos) override;
		virtual void setVelocity(const glm::vec3& vel) override;
		virtual void setAcceleration(const glm::vec3& acc) override;
		virtual void setRotation(const glm::vec3& ang) override;

		virtual void checkCollisions() = 0;
		virtual void resolveCollisions() = 0;

		virtual void setMaterial(const Material& mat) = 0;
		virtual void setMass(float mass) = 0;
		virtual void setCanCollide(bool collides) = 0;
		virtual void setCanGravity(bool canGravity) = 0;

		virtual const Material& getMaterial() const = 0;
		virtual float getMass() const = 0;
		virtual bool doesCollide() const = 0;
		virtual bool doesHaveGravity() const = 0;
		virtual UBYTE getCollisionFlags() const = 0;
	};
}