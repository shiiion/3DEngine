#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Core.h"
#include "IWorld.h"
#include "SurfaceCollisionMesh.h"
#include "IEntity.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(float w, float h, float l)
	{
		owner = nullptr;
		extents[0] = w;
		extents[1] = h;
		extents[2] = l;
	}

	MoveInfo const& CollisionMesh::getLastMove() const
	{
		return lastMove;
	}

	void CollisionMesh::generateVertexPath(float deltaTime)
	{
		//TODO: rotations
		if (owner == nullptr)
			return;
		lastMove.centerStart = owner->getParent()->getPosition();
		lastMove.centerEnd = owner->getParent()->getPosition() + owner->getParent()->getVelocity() * deltaTime;
		lastMove.velStart = owner->getParent()->getVelocity();
		lastMove.velEnd = lastMove.velStart + (lastMove.accel * deltaTime) +
			(owner->getParent()->isGravityEnabled() ? (getWorld()->getGravity() * deltaTime) : vec3(0, 0, 0));
		lastMove.accel = owner->getParent()->getAcceleration();

		cachedCenter = lastMove.centerEnd;
		cachedVel = lastMove.velEnd;
	}

	void CollisionMesh::setOwner(IPhysicsObject* owner)
	{
		this->owner = owner;
		cachedCenter = owner->getParent()->getPosition();
		cachedVel = owner->getParent()->getVelocity();
	}

	bool CollisionMesh::testCollision(ICollisionMesh const& o, float deltaTime, CollisionInfo& collisionOut)
	{
		//15 tests
		if (o.getCollisionShape() == CMESH_SHAPE_OBB)
		{
			CollisionMesh const& other = (CollisionMesh const&)o;
			for (int a = 0; a < 3; a++)
			{
				if (testAxis(axes[a], other, deltaTime) || testAxis(other.getAxis(a), other, deltaTime))
				{
					return true;
				}
				for (int b = 0; b < 3; b++)
				{
					if (testAxis(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other, deltaTime))
					{
						return true;
					}
				}
			}
			getLastSeparatingAxis(other, deltaTime, collisionOut);
			generateCollisionInfo(other, collisionOut);
			return false;
		}
		return false;
	}

	void CollisionMesh::getLastSeparatingAxis(CollisionMesh const& other, float deltaTime, CollisionInfo& collisionOut)
	{
		float longestTime = getCollisionTime(axes[0], other, deltaTime);
		float ct;
		collisionOut.lastSeparatingAxis = axes[0];
		collisionOut.lastSeparatingAxisType = AXIS_THIS_1;

		if (glm::dot(collisionOut.lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
		{
			collisionOut.intersectSide = -1;
		}
		else
		{
			collisionOut.intersectSide = 1;
		}
		for (int a = 0; a < 3; a++)
		{
			if (longestTime < (ct = getCollisionTime(axes[a], other, deltaTime)))
			{
				longestTime = ct;
				collisionOut.lastSeparatingAxis = axes[a];
				collisionOut.lastSeparatingAxisType = AXIS_THIS_1 + a;
				if (glm::dot(collisionOut.lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					collisionOut.intersectSide = -1;
				}
				else
				{
					collisionOut.intersectSide = 1;
				}
			}
			if (longestTime < (ct = getCollisionTime(other.getAxis(a), other, deltaTime)))
			{
				longestTime = ct;
				collisionOut.lastSeparatingAxis = other.getAxis(a);
				collisionOut.lastSeparatingAxisType = AXIS_OTHER_1 + a;
				if (glm::dot(collisionOut.lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					collisionOut.intersectSide = -1;
				}
				else
				{
					collisionOut.intersectSide = 1;
				}
			}
		}
		for (int a = 0; a < 3;a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (longestTime < (ct = getCollisionTime(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other, deltaTime)))
				{
					if (glm::abs(longestTime - ct) > MIN_THRESHOLD)
					{
						longestTime = ct;
						collisionOut.lastSeparatingAxis = glm::normalize(glm::cross(axes[a], other.getAxis(b)));
						collisionOut.lastSeparatingAxisType = AXIS_CROSS(a, b);
						if (glm::dot(collisionOut.lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
						{
							collisionOut.intersectSide = -1;
						}
						else
						{
							collisionOut.intersectSide = 1;
						}
					}
				}
			}
		}
		collisionOut.collisionTime = longestTime;
	}

	bool CollisionMesh::testCollisionStationary(ICollisionMesh const& o, CollisionStationary& collisionOut)
	{
		if (o.getCollisionShape() == CMESH_SHAPE_OBB)
		{
			CollisionMesh const& other = (CollisionMesh const&)o;
			for (int a = 0; a < 3; a++)
			{
				if (testAxisStationary(axes[a], other) || testAxisStationary(other.getAxis(a), other))
				{
					return true;
				}
				for (int b = 0; b < 3; b++)
				{
					if (testAxisStationary(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other))
					{
						return true;
					}
				}
			}

			collisionOut.overlapDist = getAxisOverlap(collisionOut.normal, other);
			return false;
		}
		return false;
	}

	bool CollisionMesh::testAxisStationary(vec3 const& axisNorm, CollisionMesh const& other) const
	{
		vec3 centerDiff = (other.getCachedCenter() - cachedCenter);
		float proj = glm::dot(axisNorm, centerDiff);

		float projThisBox;
		float projOtherBox;
		float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

		projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
			(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
			(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
		float r = projThisBox + projOtherBox;

		if (proj >= r)
		{
			return true;
		}
		if (proj <= -r)
		{
			return true;
		}
		return false;
	}

	float CollisionMesh::getAxisOverlap(vec3 const& axisNorm, ICollisionMesh const& o) const
	{
		if (o.getCollisionShape() == CMESH_SHAPE_OBB)
		{
			CollisionMesh const& other = (CollisionMesh const&)o;
			vec3 centerDiff = (other.getCachedCenter() - cachedCenter);
			float proj = glm::dot(axisNorm, centerDiff);

			float projThisBox;
			float projOtherBox;
			float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

			projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
				(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
				(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
			projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
				(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
				(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
			float r = projThisBox + projOtherBox;

			return r - glm::abs(proj);
		}
		return 0;
	}

//	implementation based off of http://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
//	axisNorm = L
//	centerDiff = D
//	Ai = axes of this
//	Bi = axes of other

	bool CollisionMesh::testAxis(vec3 const& axisNorm, CollisionMesh const& other, float deltaTime) const
	{
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		vec3 velDiff = (other.getLastMove().velStart - lastMove.velStart);
		float proj = glm::dot(axisNorm, centerDiff);
		float projTime = glm::dot(axisNorm, centerDiff + (velDiff * deltaTime));

		float projThisBox;
		float projOtherBox;
		float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

		projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
			(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
			(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
		float r = projThisBox + projOtherBox;

		if (proj >= r)
		{
			if (projTime >= r)
			{
				return true;
			}
		}
		if (proj <= -r)
		{
			if (projTime <= -r)
			{
				return true;
			}
		}
		return false;
	}

	float CollisionMesh::getCollisionTime(const vec3& axisNorm, const CollisionMesh& other, float deltaTime) const
	{

		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		vec3 velDiff = (other.getLastMove().velStart - lastMove.velStart);
		float proj = glm::dot(axisNorm, centerDiff);
		float projTime = glm::dot(axisNorm, centerDiff + (velDiff * deltaTime));

		float projThisBox;
		float projOtherBox;
		float otherW = other.getExtent(0), otherL = other.getExtent(2), otherH = other.getExtent(1);

		projThisBox = (extents[0] * glm::sign(glm::dot(axisNorm, axes[0])) * glm::dot(axisNorm, axes[0])) +
			(extents[1] * glm::sign(glm::dot(axisNorm, axes[1])) * glm::dot(axisNorm, axes[1])) +
			(extents[2] * glm::sign(glm::dot(axisNorm, axes[2])) * glm::dot(axisNorm, axes[2]));
		projOtherBox = (otherW * glm::sign(glm::dot(axisNorm, other.getAxis(0))) * glm::dot(axisNorm, other.getAxis(0))) +
			(otherH * glm::sign(glm::dot(axisNorm, other.getAxis(1))) * glm::dot(axisNorm, other.getAxis(1))) +
			(otherL * glm::sign(glm::dot(axisNorm, other.getAxis(2))) * glm::dot(axisNorm, other.getAxis(2)));
		float r = projThisBox + projOtherBox;

		float sigma = glm::dot(axisNorm, centerDiff);


		if (sigma < 0)
		{
			float ret = (r + glm::dot(axisNorm, centerDiff)) / (-glm::dot(axisNorm, velDiff));
			if (ret > deltaTime)
			{
				return -1;
			}
			else
			{
				return ret;
			}
		}
		if (sigma > 0)
		{
			float ret = (r - glm::dot(axisNorm, centerDiff)) / (glm::dot(axisNorm, velDiff));
			if (ret > deltaTime)
			{
				return -1;
			}
			else
			{
				return ret;
			}
		}
		return -1;
	}

	vec3 const& CollisionMesh::getAxis(int axis) const
	{
		return axes[axis];
	}

	float CollisionMesh::getExtent(int extent) const
	{
		return extents[extent];
	}

	void CollisionMesh::this_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
#define CIJ(i, j) (glm::dot(axes[i], other.getAxis(j)))
		int index = 0;
		switch (collisionOut.lastSeparatingAxisType)
		{
		case AXIS_THIS_1:
			index = 0;
			break;
		case AXIS_THIS_2:
			index = 1;
			break;
		case AXIS_THIS_3:
			index = 2;
			break;
		}
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;

		collisionOut.collisionNormal = axes[index] * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = axes[index];

		MoveInfo const& otherMove = other.getLastMove();
		vec3 CAI = otherMove.centerStart + (otherMove.velStart * collisionOut.collisionTime);
		float sumx = 0, sumy = 0, sumz = 0, minyx, minyy, minyz, maxyx, maxyy, maxyz;
		for (int a = 0; a < 3; a++)
		{
			sumx += glm::abs(CIJ(a, 0)) * extents[a];
			sumy += glm::abs(CIJ(a, 1)) * extents[a];
			sumz += glm::abs(CIJ(a, 2)) * extents[a];
		}

		minyx = glm::dot(-other.getAxis(0), centerDiff) - sumx;
		minyy = glm::dot(-other.getAxis(1), centerDiff) - sumy;
		minyz = glm::dot(-other.getAxis(2), centerDiff) - sumz;
		maxyx = glm::dot(-other.getAxis(0), centerDiff) + sumx;
		maxyy = glm::dot(-other.getAxis(1), centerDiff) + sumy;
		maxyz = glm::dot(-other.getAxis(2), centerDiff) + sumz;

		float x, y, z;
		if (other.getExtent(0) <= maxyx)
		{
			x = other.getExtent(0);
		}
		else if (-other.getExtent(0) >= minyx)
		{
			x = -other.getExtent(0);
		}
		else
		{
			x = minyx;
		}
			
		if (other.getExtent(1) <= maxyy)
		{
			y = other.getExtent(1);
		}
		else if (-other.getExtent(1) >= minyy)
		{
			y = -other.getExtent(1);
		}
		else
		{
			y = minyy;
		}
		
		if (other.getExtent(2) <= maxyz)
		{
			z = other.getExtent(2);
		}
		else if (-other.getExtent(2) >= minyz)
		{
			z = -other.getExtent(2);
		}
		else
		{
			z = minyz;
		}
		collisionOut.intersectionPoint = ((x * other.getAxis(0)) + (y * other.getAxis(1)) + (z * other.getAxis(2))) + CAI;
#undef CIJ
	}

	void CollisionMesh::other_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
#define CJI(j, i) (glm::dot(axes[j], other.getAxis(i)))
		int index = 0;
		bool dotProductZero = false;
		switch (collisionOut.lastSeparatingAxisType)
		{
		case AXIS_OTHER_1:
			index = 0;
			break;
		case AXIS_OTHER_2:
			index = 1;
			break;
		case AXIS_OTHER_3:
			index = 2;
			break;
		}
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;

		collisionOut.collisionNormal = other.getAxis(index) * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = other.getAxis(index);

		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		float sumx = 0, sumy = 0, sumz = 0, minxx, minxy, minxz, maxxx, maxxy, maxxz;
		for (int a = 0; a < 3; a++)
		{
			sumx += glm::abs(CJI(0, a)) * other.getExtent(a);
			sumy += glm::abs(CJI(1, a)) * other.getExtent(a);
			sumz += glm::abs(CJI(2, a)) * other.getExtent(a);
		}

		minxx = glm::dot(axes[0], centerDiff) - sumx;
		minxy = glm::dot(axes[1], centerDiff) - sumy;
		minxz = glm::dot(axes[2], centerDiff) - sumz;
		maxxx = glm::dot(axes[0], centerDiff) + sumx;
		maxxy = glm::dot(axes[1], centerDiff) + sumy;
		maxxz = glm::dot(axes[2], centerDiff) + sumz;

		float x, y, z;
		if (other.getExtent(0) <= maxxx)
		{
			x = other.getExtent(0);
		}
		else if (-other.getExtent(0) >= minxx)
		{
			x = -other.getExtent(0);
		}
		else
		{
			x = minxx;
		}

		if (other.getExtent(1) <= maxxy)
		{
			y = other.getExtent(1);
		}
		else if (-other.getExtent(1) >= minxy)
		{
			y = -other.getExtent(1);
		}
		else
		{
			y = minxy;
		}

		if (other.getExtent(2) <= maxxz)
		{
			z = other.getExtent(2);
		}
		else if (-other.getExtent(2) >= minxz)
		{
			z = -other.getExtent(2);
		}
		else
		{
			z = minxz;
		}
		collisionOut.intersectionPoint = ((x * axes[0]) + (y * axes[1]) + (z * axes[2])) + CAI;
#undef CJI
	}

#pragma region xproduct info generation
#define CIJ(i, j) (glm::dot(axes[i], other.getAxis(j)))

	void CollisionMesh::A0xB0_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x1 = -collisionOut.intersectSide * glm::sign(CIJ(2, 0)) * extents[1];
		float x2 = collisionOut.intersectSide * glm::sign(CIJ(1, 0)) * extents[2];
		float y1 = -collisionOut.intersectSide * glm::sign(CIJ(0, 2)) * other.getExtent(1);
		float y2 = collisionOut.intersectSide * glm::sign(CIJ(0, 1)) * other.getExtent(2);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 0), 2.f))) *
			(glm::dot(axes[0], centerDiff) + 
			(CIJ(0, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(1, 0) * x1) + (CIJ(2, 0) * x2))) +
			(CIJ(0, 1) * y1) + (CIJ(0, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A0xB1_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x1 = -collisionOut.intersectSide * glm::sign(CIJ(2, 1)) * extents[1];
		float x2 = collisionOut.intersectSide * glm::sign(CIJ(1, 1)) * extents[2];
		float y0 = collisionOut.intersectSide * glm::sign(CIJ(0, 2)) * other.getExtent(0);
		float y2 = -collisionOut.intersectSide * glm::sign(CIJ(0, 0)) * other.getExtent(2);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 1), 2.f))) *
			(glm::dot(axes[0], centerDiff) +
				(CIJ(0, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(1, 1) * x1) + (CIJ(2, 1) * x2))) +
				(CIJ(0, 0) * y0) + (CIJ(0, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A0xB2_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut) 
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x1 = -collisionOut.intersectSide * glm::sign(CIJ(2, 2)) * extents[1];
		float x2 = collisionOut.intersectSide * glm::sign(CIJ(1, 2)) * extents[2];
		float y0 = -collisionOut.intersectSide * glm::sign(CIJ(0, 1)) * other.getExtent(0);
		float y1 = collisionOut.intersectSide * glm::sign(CIJ(0, 0)) * other.getExtent(1);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 2), 2.f))) *
			(glm::dot(axes[0], centerDiff) +
				(CIJ(0, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(1, 2) * x1) + (CIJ(2, 2) * x2))) +
				(CIJ(0, 0) * y0) + (CIJ(0, 1) * y1));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB0_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = collisionOut.intersectSide * glm::sign(CIJ(2, 0)) * extents[0];
		float x2 = -collisionOut.intersectSide * glm::sign(CIJ(0, 0)) * extents[2];
		float y1 = -collisionOut.intersectSide * glm::sign(CIJ(1, 2)) * other.getExtent(1);
		float y2 = collisionOut.intersectSide * glm::sign(CIJ(1, 1)) * other.getExtent(2);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 0), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(0, 0) * x0) + (CIJ(2, 0) * x2))) +
				(CIJ(1, 1) * y1) + (CIJ(1, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB1_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = collisionOut.intersectSide * glm::sign(CIJ(2, 1)) * extents[0];
		float x2 = -collisionOut.intersectSide * glm::sign(CIJ(0, 1)) * extents[2];
		float y0 = collisionOut.intersectSide * glm::sign(CIJ(1, 2)) * other.getExtent(0);
		float y2 = -collisionOut.intersectSide * glm::sign(CIJ(1, 0)) * other.getExtent(2);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 1), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(0, 1) * x0) + (CIJ(2, 1) * x2))) +
				(CIJ(1, 0) * y0) + (CIJ(1, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB2_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = collisionOut.intersectSide * glm::sign(CIJ(2, 2)) * extents[0];
		float x2 = -collisionOut.intersectSide * glm::sign(CIJ(0, 2)) * extents[2];
		float y0 = -collisionOut.intersectSide * glm::sign(CIJ(1, 1)) * other.getExtent(0);
		float y1 = collisionOut.intersectSide * glm::sign(CIJ(1, 0)) * other.getExtent(1);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 2), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(0, 2) * x0) + (CIJ(2, 2) * x2))) +
				(CIJ(1, 0) * y0) + (CIJ(1, 1) * y1));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB0_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = -collisionOut.intersectSide * glm::sign(CIJ(1, 0)) * extents[0];
		float x1 = collisionOut.intersectSide * glm::sign(CIJ(0, 0)) * extents[1];
		float y1 = -collisionOut.intersectSide * glm::sign(CIJ(2, 2)) * other.getExtent(1);
		float y2 = collisionOut.intersectSide * glm::sign(CIJ(2, 1)) * other.getExtent(2);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 0), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(0, 0) * x0) + (CIJ(1, 0) * x1))) +
				(CIJ(2, 1) * y1) + (CIJ(2, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB1_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = -collisionOut.intersectSide * glm::sign(CIJ(1, 1)) * extents[0];
		float x1 = collisionOut.intersectSide * glm::sign(CIJ(0, 1)) * extents[1];
		float y0 = collisionOut.intersectSide * glm::sign(CIJ(2, 2)) * other.getExtent(0);
		float y2 = -collisionOut.intersectSide * glm::sign(CIJ(2, 0)) * other.getExtent(2);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 1), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(0, 1) * x0) + (CIJ(1, 1) * x1))) +
				(CIJ(2, 0) * y0) + (CIJ(2, 2) * y2));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB2_lastSeparatingAxis(CollisionMesh const& other, CollisionInfo& collisionOut)
	{
		vec3 CAI = (lastMove.centerStart + (lastMove.velStart * collisionOut.collisionTime));
		vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velStart) * collisionOut.collisionTime;
		float x0 = -collisionOut.intersectSide * glm::sign(CIJ(1, 2)) * extents[0];
		float x1 = collisionOut.intersectSide * glm::sign(CIJ(0, 2)) * extents[1];
		float y0 = -collisionOut.intersectSide * glm::sign(CIJ(2, 1)) * other.getExtent(0);
		float y1 = collisionOut.intersectSide * glm::sign(CIJ(2, 0)) * other.getExtent(1);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 2), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(0, 2) * x0) + (CIJ(1, 2) * x1))) +
				(CIJ(2, 0) * y0) + (CIJ(2, 1) * y1));

		collisionOut.collisionNormal = collisionOut.lastSeparatingAxis * (float)-collisionOut.intersectSide;
		collisionOut.lastSeparatingAxis = collisionOut.lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}
#undef CIJ
#pragma endregion

	void CollisionMesh::generateCollisionInfo(ICollisionMesh const& o, CollisionInfo& collisionOut)
	{
		if (o.getCollisionShape() == CMESH_SHAPE_OBB)
		{
			CollisionMesh const& other = (CollisionMesh const&)o;
			switch (collisionOut.lastSeparatingAxisType)
			{
				//FACE V ?
				//? V FACE
			case AXIS_THIS_1:
			case AXIS_THIS_2:
			case AXIS_THIS_3:
				this_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_OTHER_1:
			case AXIS_OTHER_2:
			case AXIS_OTHER_3:
				other_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_1X1:
				A0xB0_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_1X2:
				A0xB1_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_1X3:
				A0xB2_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_2X1:
				A1xB0_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_2X2:
				A1xB1_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_2X3:
				A1xB2_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_3X1:
				A2xB0_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_3X2:
				A2xB1_lastSeparatingAxis(other, collisionOut);
				break;
			case AXIS_3X3:
				A2xB2_lastSeparatingAxis(other, collisionOut);
				break;
			}
		}
	}

	IPhysicsObject* CollisionMesh::getOwner() const
	{
		return owner;
	}

	void CollisionMesh::setCachedCenter(vec3 const& center)
	{
		this->cachedCenter = center;
	}
	
	vec3 const& CollisionMesh::getCachedCenter() const
	{
		return cachedCenter;
	}

	void CollisionMesh::setCachedVelocity(vec3 const& vel)
	{
		this->cachedVel = vel;
	}

	vec3 const& CollisionMesh::getCachedVelocity() const
	{
		return cachedVel;
	}

	ICollisionMesh* createCollisionMesh(float w, float h, float l)
	{
		return new CollisionMesh(w, h, l);
	}

	bool CollisionMesh::testRay(RaytraceParams& params, RaytraceResult& resultOut) const
	{
		SurfaceCollisionMesh surfaceMesh(this);
		for (int a = 0; a < 6; a++)
		{
			if (surfaceMesh.faces[a].intersectsWithSurface(resultOut.ray, resultOut.rayDist))
			{
				if (params.func != nullptr)
				{
					if (params.func(getOwner()))
					{
						resultOut.didHit = true;
						resultOut.firstCollision = getOwner();
						resultOut.collisionDist = surfaceMesh.faces[a].getIntersectionValue(resultOut.ray);
						return true;
					}
				}
				else
				{
					resultOut.didHit = true;
					resultOut.firstCollision = getOwner();
					resultOut.collisionDist = surfaceMesh.faces[a].getIntersectionValue(resultOut.ray);
					return true;
				}
			}
		}
		return false;
	}
}