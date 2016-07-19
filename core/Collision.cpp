#include "Collision.h"
#include "ICollisionMesh.h"
#include "IPhysicsObject.h"

namespace ginkgo
{
	Collision::Collision(float deltaTime, CollisionInfo const& manifold)
		: deltaTime(deltaTime), manifold(manifold.thisMesh, manifold.otherMesh)
	{
		valid = true;
		referenceResult.finalPos = manifold.thisMesh->getLastMove().centerEnd;
		referenceResult.finalVel = manifold.thisMesh->getOwner()->getParent()->getVelocity();
		otherResult.finalPos = manifold.otherMesh->getLastMove().centerEnd;
		otherResult.finalVel = manifold.otherMesh->getOwner()->getParent()->getVelocity();
		this->manifold.normal = manifold.collisionNormal;
		this->manifold.overlapDist = manifold.thisMesh->getAxisOverlap(manifold.collisionNormal, *manifold.otherMesh);
	}

	void Collision::impulseCorrection()
	{
		IPhysicsObject* refObj = manifold.thisMesh->getOwner(), *otherObj = manifold.otherMesh->getOwner();
		float mass = refObj->getMass(), otherMass = otherObj->getMass();

		glm::vec3 const& vel = referenceResult.finalVel;
		glm::vec3 const& otherVel = otherResult.finalVel;

		//Contact velocity
		float contactVel = glm::dot(otherVel - vel, manifold.normal);

		if (contactVel < 0)
		{
			return;
		}

		//select minimum restitution
		float restitution = glm::min(otherObj->getMaterial().reboundFraction, refObj->getMaterial().reboundFraction);

		//impulse scalar
		//if type is worldstatic, mass is infinite (1/mass = 0)
		float IS = -(1.f + restitution) * contactVel;
		float invMassThis, invMassOther;

		if (refObj->getCollisionType() == CTYPE_WORLDSTATIC)
		{
			invMassThis = 0;
		}
		else
		{
			invMassThis = 1 / mass;
		}

		if (otherObj->getCollisionType() == CTYPE_WORLDSTATIC)
		{
			invMassOther = 0;
		}
		else
		{
			invMassOther = 1 / otherMass;
		}

		IS /= (invMassThis + invMassOther);

		if (glm::abs(IS) < /*MINREBOUND*/0.01f)
		{
			IS = (-contactVel) / (invMassThis + invMassOther);
		}

		glm::vec3 impulse = manifold.normal * IS;

		referenceResult.finalVel -= (invMassThis * impulse);
		otherResult.finalVel += (invMassOther * impulse);
	}

	void Collision::positionalCorrectionInternal(float frameSegment)
	{
		float invMassRef = manifold.thisMesh->getOwner()->getMass();
		float invMassOther = manifold.otherMesh->getOwner()->getMass();
		if (manifold.thisMesh->getOwner()->getCollisionType() == CTYPE_WORLDSTATIC)
		{
			invMassRef = 0;
		}
		else
		{
			invMassRef = 1.f / invMassRef;
		}

		if (manifold.otherMesh->getOwner()->getCollisionType() == CTYPE_WORLDSTATIC)
		{
			invMassOther = 0;
		}
		else
		{
			invMassOther = 1.f / invMassOther;
		}

		glm::vec3 correction = (glm::max(manifold.overlapDist - MIN_CORRECTDIST, 0.f) / (invMassRef + invMassOther)) * frameSegment * manifold.normal;
		referenceResult.finalPos += invMassRef * correction;
		otherResult.finalPos -= invMassOther * correction;

		manifold.thisMesh->setCenter(referenceResult.finalPos);
		manifold.otherMesh->setCenter(otherResult.finalPos);
	}

	void Collision::positionalCorrection(float correctionPercent)
	{
		if (!valid)
		{
			return;
		}
		impulseCorrection();
		positionalCorrectionInternal(correctionPercent);
		updateValidity();
	}

	void Collision::postCorrection()
	{
		manifold.thisMesh->getOwner()->setFinalMove(referenceResult);
		manifold.otherMesh->getOwner()->setFinalMove(otherResult);
	}

	void Collision::updateValidity()
	{
		valid = !manifold.thisMesh->testCollisionStationary(*manifold.otherMesh, manifold);
	}

	bool Collision::eq(IPhysicsObject* a, IPhysicsObject* b) const
	{
		long IDt = manifold.thisMesh->getOwner()->getParent()->getEntityID();
		long IDo = manifold.otherMesh->getOwner()->getParent()->getEntityID();
		long IDa = a->getParent()->getEntityID();
		long IDb = b->getParent()->getEntityID();

		return (IDt == IDa && IDo == IDb) || (IDt == IDb && IDo == IDa);
	}
}