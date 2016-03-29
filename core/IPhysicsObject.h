#pragma once

#include "IRenderable.h"

namespace ginkgo
{
	class ICollisionMesh;

	class IPhysicsObject : public IRenderable
	{
	public:
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