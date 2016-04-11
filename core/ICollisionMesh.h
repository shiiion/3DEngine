#pragma once

#include "CoreReource.h"

#define CMESH_NUM_SURFACES 6
#define CMESH_NUM_VERTICES 8

namespace ginkgo
{
	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual glm::vec3 const* getBoundingVertices() const = 0;
		virtual MoveInfo const& getLastMove() const = 0;
		virtual void generateVertexPath(float deltaTime) = 0;

		virtual void setOwner(IPhysicsObject const* owner) = 0;

		virtual bool testCollision(ICollisionMesh const& other, float deltaTime) = 0;

		virtual float getCollisionTime(ICollisionMesh const& other, float deltaTime) const = 0;

		virtual void finalizeMove() = 0;

		virtual glm::vec3 const& getWidthAxis() const = 0;
		virtual glm::vec3 const& getHeightAxis() const = 0;
		virtual glm::vec3 const& getLengthAxis() const = 0;
		virtual glm::vec3 const& getCenter() const = 0;
		virtual float getLengthExtent() const = 0;
		virtual float getWidthExtent() const = 0;
		virtual float getHeightExtent() const = 0;
		virtual void generateCollisionInfo(ICollisionMesh const& other, float intersectTime) = 0;
	};

	DECLSPEC_CORE ICollisionMesh* createCollisionMesh(float l, float w, float h);
}