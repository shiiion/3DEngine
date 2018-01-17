
#include "Octree.h"
#include "IPhysicsComponent.h"
#include "IEntity.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

namespace ginkgo
{
	Octree::Octree(int level, Prism const& bounds, Octree* parent)
	{
		this->level = level;
		objects.reserve(OCTREE_MAXENTS + 1);
		this->bounds = bounds;
		this->parent = parent;
		for (int a = 0; a < 8; a++)
		{
			leaves[a] = nullptr;
		}
	}

	Octree::~Octree()
	{
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	
}