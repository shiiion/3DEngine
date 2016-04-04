#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual ISurface const* const* getFaces() const = 0;
		virtual glm::vec3 const* getBoundingVertices() const = 0;
		virtual void generateVertexPath(float deltaTime) = 0;

		virtual void setOwner(IPhysicsObject const* owner) = 0;
	};
}