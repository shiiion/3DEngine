#pragma once

#include "CoreReource.h"
#include "Surface.h"
#include "CollisionMesh.h"

//	collision mesh represented by 6 surfaces

namespace ginkgo
{
	struct SurfaceCollisionMesh
	{
		SurfaceCollisionMesh(CollisionMesh const* mesh)
			: mesh(mesh)
		{
			createSurfacesFromMesh();
		}

		void createSurfacesFromMesh();

		Surface faces[6];
		CollisionMesh const* mesh;
	};

	
}