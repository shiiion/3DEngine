#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface;

	class ICollisionMesh
	{
	public:
		virtual void getFaces(vector<ISurface*>& surfaceList) const;

	};
}