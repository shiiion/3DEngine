#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual ISurface** getFaces() const = 0;
		virtual void getBoundingVertices(vector<glm::vec3>& vertexList) const = 0;
		virtual void generateVertexPath(float deltaTime) = 0;

		virtual void setOwner(IPhysicsObject const* owner) = 0;
	};
}