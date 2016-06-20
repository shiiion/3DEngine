#pragma once

#include "CoreReource.h"
#include "Surface.h"
#include "ICollisionMesh.h"

//	collision mesh represented by 6 surfaces

namespace ginkgo
{
	struct SurfaceCollisionMesh
	{
		SurfaceCollisionMesh(ICollisionMesh* mesh)
			: mesh(mesh)
		{
			createSurfacesFromMesh();
		}

		void createSurfacesFromMesh();

		Surface faces[6];
		ICollisionMesh* mesh;
	};

	
}