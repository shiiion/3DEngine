#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IPhysicsComponent;
	class IEntity;
#define OCTREE_MAXENTS 16
#define OCTREE_MAXLEVELS 20

#define REMOVE_NOTFOUND 1
#define REMOVE_FOUNDNOTEMPTY 2
#define REMOVE_FOUNDEMPTY 3
#define REMOVE_FOUNDBYCHILD 4

//	collision test management
	class Octree
	{
	private:
		int level;
		vector<IPhysicsComponent*> objects;
		Prism bounds;
		Octree* leaves[8];
		Octree* parent;
	public:
		Octree(int level, Prism const& bounds, Octree* parent);
		

		~Octree();
	};
}