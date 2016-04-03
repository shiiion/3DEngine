#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface;
	class IPhysicsObject;

	class ICollisionMesh
	{
	public:
		virtual void getFaces(vector<ISurface*>& surfaceList) const = 0;
		virtual void getBoundingVertices(vector<glm::vec3>& vertexList) const = 0;

		virtual void generateVertexPath(float deltaTime, IPhysicsObject* owner) = 0;
	};
}