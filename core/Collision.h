#pragma once
#include "CoreReource.h"

namespace ginkgo
{
	class IPhysicsObject;

	struct Collision
	{
		CollisionStationary manifold;

		float deltaTime;

		MoveResult referenceResult;
		MoveResult otherResult;

		const float MIN_CORRECTDIST = 0.01f;

		bool valid;
	private:
		void positionalCorrectionInternal(float frameSegment);

	public:

		Collision(float deltaTime, CollisionInfo const& manifold);

		void impulseCorrection();

		//suggested for testing with 20% correction (0.5)
		void positionalCorrection(float correctionPerIteration);

		void postCorrection();

		void updateValidity();

		bool eq(IPhysicsObject* a, IPhysicsObject* b) const;

		void getUpdatedParams();

		bool preCorrectionCheck()
		{
			getUpdatedParams();
			updateValidity();
			return manifold.overlapDist >= MIN_CORRECTDIST;
		}

		Collision& operator=(Collision const& other)
		{
			manifold = other.manifold;
			deltaTime = other.deltaTime;
			referenceResult = other.referenceResult;
			otherResult = other.otherResult;
			valid = other.valid;
			return *this;
		}
	};
}