#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Core.h"
#include "IWorld.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis)
		: lastCollision(nullptr, nullptr)
	{
		owner = nullptr;
		extents[0] = w;
		extents[1] = h;
		extents[2] = l;

		axes[0] = wAxis;
		axes[1] = hAxis;
		axes[2] = lAxis;
		newSeparatingAxis = true;
	}

	glm::vec3 const* CollisionMesh::getBoundingVertices() const
	{
		return nullptr;
	}

	MoveInfo const& CollisionMesh::getLastMove() const
	{
		return lastMove;
	}

	void CollisionMesh::finalizeMove()
	{
		center = owner->getPosition();

	}

	void CollisionMesh::generateVertexPath(float deltaTime)
	{
		//TODO: rotations
		if (owner == nullptr)
			return;
		lastMove.centerStart = owner->getPosition();
		lastMove.centerEnd = owner->getPosition() + owner->getVelocity() * deltaTime;
		lastMove.velStart = owner->getVelocity();
		lastMove.velEnd = lastMove.velStart + (lastMove.accel * deltaTime) - (getWorld()->getGravity() * deltaTime);
		lastMove.accel = owner->getAcceleration();

		center = lastMove.centerEnd;
	}

	void CollisionMesh::setOwner(IPhysicsObject* owner)
	{
		this->owner = owner;
		center = owner->getPosition();
	}

	bool CollisionMesh::testCollision(ICollisionMesh const& other, float deltaTime)
	{
		//15 tests

		for (int a = 0; a < 3; a++)
		{
			if (testAxis(axes[a], other, deltaTime) || testAxis(other.getAxis(a), other, deltaTime))
			{
				return newSeparatingAxis = true;
			}
			for (int b = 0; b < 3; b++)
			{
				if(testAxis(glm::normalize(glm::cross(axes[a], other.getAxis(b))), other, deltaTime))
				{
					return newSeparatingAxis = true;
				}
			}
		}

		if (newSeparatingAxis)
		{
			newSeparatingAxis = false;
			getLastSeparatingAxis(other, deltaTime);
			lastCollision = generateCollisionInfo(other, getCollisionTime(lastSeparatingAxis, other, deltaTime));
		}
		else
		{
			//THIS ASSUMES THAT THEY ARE AGAINST EACHOTHER I AM HOPING CHANGE THIS IF IT BREAKS LATER
			float intersectTime = getCollisionTime(lastSeparatingAxis, other, deltaTime);
			if (intersectTime == 0 || (intersectTime != intersectTime))
			{
				return true;
			}
		}
		return false;
	}

	void CollisionMesh::getLastSeparatingAxis(ICollisionMesh const& other, float deltaTime)
	{
		float longestTime = getCollisionTime(axes[0], other, deltaTime);
		float ct;
		lastSeparatingAxis = axes[0];
		lastSeparatingAxisType = AXIS_THIS_1;

		if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
		{
			intersectSide = -1;
		}
		else
		{
			intersectSide = 1;
		}
		for (int a = 0; a < 3; a++)
		{
			if (longestTime < (ct = getCollisionTime(axes[a], other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = axes[a];
				lastSeparatingAxisType = AXIS_THIS_1 + a;
				if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					intersectSide = -1;
				}
				else
				{
					intersectSide = 1;
				}
			}
			if (longestTime < (ct = getCollisionTime(other.getAxis(a), other, deltaTime)))
			{
				longestTime = ct;
				lastSeparatingAxis = other.getAxis(a);
				lastSeparatingAxisType = AXIS_OTHER_1 + a;
				if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
				{
					intersectSide = -1;
				}
				else
				{
					intersectSide = 1;
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
						lastSeparatingAxis = glm::normalize(glm::cross(axes[a], other.getAxis(b)));
						lastSeparatingAxisType = AXIS_CROSS(a, b);
						if (glm::dot(lastSeparatingAxis, other.getLastMove().centerStart - lastMove.centerStart) < 0)
						{
							intersectSide = -1;
						}
						else
						{
							intersectSide = 1;
						}
					}
				}
			}
		}
	}

//	implementation based off of http://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
//	axisNorm = L
//	centerDiff = D
//	Ai = axes of this
//	Bi = axes of other

	bool CollisionMesh::testAxis(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime) const
	{
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		glm::vec3 velDiff = (other.getLastMove().centerStart - lastMove.velStart);
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

		if (proj > r)
		{
			if (projTime > r)
			{
				return true;
			}
		}
		if (proj < -r)
		{
			if (projTime < -r)
			{
				return true;
			}
		}
		return false;
	}

	float CollisionMesh::getCollisionTime(const glm::vec3& axisNorm, const ICollisionMesh& other, float deltaTime) const
	{

		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart);
		glm::vec3 velDiff = (other.getLastMove().centerStart - lastMove.velStart);
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
				return 0;
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
				return 0;
			}
			else
			{
				return ret;
			}
		}
		//uh oh
		return 0;
	}

	glm::vec3 const& CollisionMesh::getAxis(int axis) const
	{
		return axes[axis];
	}

	float CollisionMesh::getExtent(int extent) const
	{
		return extents[extent];
	}

	glm::vec3 const& CollisionMesh::getCenter() const
	{
		return center;
	}

	void CollisionMesh::this_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
#define CIJ(i, j) (glm::dot(axes[i], other.getAxis(j)))
		int index = 0;
		switch (lastSeparatingAxisType)
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
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;

		collisionOut.collisionNormal = axes[index] * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = axes[index];

		MoveInfo const& otherMove = other.getLastMove();
		glm::vec3 CAI = otherMove.centerStart + (otherMove.velStart * intersectTime);
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

	void CollisionMesh::other_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
#define CJI(j, i) (glm::dot(axes[j], other.getAxis(i)))
		int index = 0;
		bool dotProductZero = false;
		switch (lastSeparatingAxisType)
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
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;

		collisionOut.collisionNormal = other.getAxis(index) * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = other.getAxis(index);

		
	
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
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

	void CollisionMesh::A0xB0_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x1 = -intersectSide * glm::sign(CIJ(2, 0)) * extents[1];
		float x2 = intersectSide * glm::sign(CIJ(1, 0)) * extents[2];
		float y1 = -intersectSide * glm::sign(CIJ(0, 2)) * other.getExtent(1);
		float y2 = intersectSide * glm::sign(CIJ(0, 1)) * other.getExtent(2);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 0), 2.f))) *
			(glm::dot(axes[0], centerDiff) + 
			(CIJ(0, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(1, 0) * x1) + (CIJ(2, 0) * x2))) +
			(CIJ(0, 1) * y1) + (CIJ(0, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A0xB1_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x1 = -intersectSide * glm::sign(CIJ(2, 1)) * extents[1];
		float x2 = intersectSide * glm::sign(CIJ(1, 1)) * extents[2];
		float y0 = intersectSide * glm::sign(CIJ(0, 2)) * other.getExtent(0);
		float y2 = -intersectSide * glm::sign(CIJ(0, 0)) * other.getExtent(2);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 1), 2.f))) *
			(glm::dot(axes[0], centerDiff) +
				(CIJ(0, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(1, 1) * x1) + (CIJ(2, 1) * x2))) +
				(CIJ(0, 0) * y0) + (CIJ(0, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A0xB2_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut) 
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x1 = -intersectSide * glm::sign(CIJ(2, 2)) * extents[1];
		float x2 = intersectSide * glm::sign(CIJ(1, 2)) * extents[2];
		float y0 = -intersectSide * glm::sign(CIJ(0, 1)) * other.getExtent(0);
		float y1 = intersectSide * glm::sign(CIJ(0, 0)) * other.getExtent(1);

		float x0 = (1.f / (1.f - glm::pow(CIJ(0, 2), 2.f))) *
			(glm::dot(axes[0], centerDiff) +
				(CIJ(0, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(1, 2) * x1) + (CIJ(2, 2) * x2))) +
				(CIJ(0, 0) * y0) + (CIJ(0, 1) * y1));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB0_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = intersectSide * glm::sign(CIJ(2, 0)) * extents[0];
		float x2 = -intersectSide * glm::sign(CIJ(0, 0)) * extents[2];
		float y1 = -intersectSide * glm::sign(CIJ(1, 2)) * other.getExtent(1);
		float y2 = intersectSide * glm::sign(CIJ(1, 1)) * other.getExtent(2);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 0), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(0, 0) * x0) + (CIJ(2, 0) * x2))) +
				(CIJ(1, 1) * y1) + (CIJ(1, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB1_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = intersectSide * glm::sign(CIJ(2, 1)) * extents[0];
		float x2 = -intersectSide * glm::sign(CIJ(0, 1)) * extents[2];
		float y0 = intersectSide * glm::sign(CIJ(1, 2)) * other.getExtent(0);
		float y2 = -intersectSide * glm::sign(CIJ(1, 0)) * other.getExtent(2);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 1), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(0, 1) * x0) + (CIJ(2, 1) * x2))) +
				(CIJ(1, 0) * y0) + (CIJ(1, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A1xB2_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = intersectSide * glm::sign(CIJ(2, 2)) * extents[0];
		float x2 = -intersectSide * glm::sign(CIJ(0, 2)) * extents[2];
		float y0 = -intersectSide * glm::sign(CIJ(1, 1)) * other.getExtent(0);
		float y1 = intersectSide * glm::sign(CIJ(1, 0)) * other.getExtent(1);

		float x1 = (1.f / (1.f - glm::pow(CIJ(1, 2), 2.f))) *
			(glm::dot(axes[1], centerDiff) +
				(CIJ(1, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(0, 2) * x0) + (CIJ(2, 2) * x2))) +
				(CIJ(1, 0) * y0) + (CIJ(1, 1) * y1));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB0_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = -intersectSide * glm::sign(CIJ(1, 0)) * extents[0];
		float x1 = intersectSide * glm::sign(CIJ(0, 0)) * extents[1];
		float y1 = -intersectSide * glm::sign(CIJ(2, 2)) * other.getExtent(1);
		float y2 = intersectSide * glm::sign(CIJ(2, 1)) * other.getExtent(2);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 0), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 0) * (glm::dot(-other.getAxis(0), centerDiff) + (CIJ(0, 0) * x0) + (CIJ(1, 0) * x1))) +
				(CIJ(2, 1) * y1) + (CIJ(2, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB1_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = -intersectSide * glm::sign(CIJ(1, 1)) * extents[0];
		float x1 = intersectSide * glm::sign(CIJ(0, 1)) * extents[1];
		float y0 = intersectSide * glm::sign(CIJ(2, 2)) * other.getExtent(0);
		float y2 = -intersectSide * glm::sign(CIJ(2, 0)) * other.getExtent(2);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 1), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 1) * (glm::dot(-other.getAxis(1), centerDiff) + (CIJ(0, 1) * x0) + (CIJ(1, 1) * x1))) +
				(CIJ(2, 0) * y0) + (CIJ(2, 2) * y2));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}

	void CollisionMesh::A2xB2_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut)
	{
		glm::vec3 CAI = (lastMove.centerStart + (lastMove.velStart * intersectTime));
		glm::vec3 centerDiff = (other.getLastMove().centerStart - lastMove.centerStart) + (other.getLastMove().velStart - lastMove.velEnd) * intersectTime;
		float x0 = -intersectSide * glm::sign(CIJ(1, 2)) * extents[0];
		float x1 = intersectSide * glm::sign(CIJ(0, 2)) * extents[1];
		float y0 = -intersectSide * glm::sign(CIJ(2, 1)) * other.getExtent(0);
		float y1 = intersectSide * glm::sign(CIJ(2, 0)) * other.getExtent(1);

		float x2 = (1.f / (1.f - glm::pow(CIJ(2, 2), 2.f))) *
			(glm::dot(axes[2], centerDiff) +
				(CIJ(2, 2) * (glm::dot(-other.getAxis(2), centerDiff) + (CIJ(0, 2) * x0) + (CIJ(1, 2) * x1))) +
				(CIJ(2, 0) * y0) + (CIJ(2, 1) * y1));

		collisionOut.collisionNormal = lastSeparatingAxis * (float)-intersectSide;
		collisionOut.lastSeparatingAxis = lastSeparatingAxis;
		collisionOut.intersectionPoint = ((x0 * axes[0]) + (x1 * axes[1]) + (x2 * axes[2])) + CAI;
	}
#undef CIJ
#pragma endregion

	CollisionInfo CollisionMesh::generateCollisionInfo(ICollisionMesh const& other, float intersectTime)
	{
		CollisionInfo ret(const_cast<CollisionMesh*>(this), const_cast<ICollisionMesh*>(&other));
		ret.collisionTime = intersectTime;
		switch (lastSeparatingAxisType)
		{
		//FACE V ?
		//? V FACE
		case AXIS_THIS_1:
		case AXIS_THIS_2:
		case AXIS_THIS_3:
			this_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_OTHER_1:
		case AXIS_OTHER_2:
		case AXIS_OTHER_3:
			other_lastSeparatingAxis(other, intersectTime, ret);
			break;
		//EDGE V EDGE OH SHIT
		case AXIS_1X1:
			A0xB0_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_1X2:
			A0xB1_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_1X3:
			A0xB2_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_2X1:
			A1xB0_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_2X2:
			A1xB1_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_2X3:
			A1xB2_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_3X1:
			A2xB0_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_3X2:
			A2xB1_lastSeparatingAxis(other, intersectTime, ret);
			break;
		case AXIS_3X3:
			A2xB2_lastSeparatingAxis(other, intersectTime, ret);
			break;
		}
		return ret;
	}

	IPhysicsObject* CollisionMesh::getOwner() const
	{
		return owner;
	}

	//TODO
	CollisionInfo const& CollisionMesh::resolveCollision() 
	{
		center = lastMove.centerStart + (lastMove.velStart * lastCollision.collisionTime);
		return lastCollision;
	}

	ICollisionMesh* createCollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis)
	{
		return new CollisionMesh(w, h, l, wAxis, hAxis, lAxis);
	}
}