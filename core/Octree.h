#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IPhysicsObject;
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
		vector<IPhysicsObject*> objects;
		Prism bounds;
		Octree* leaves[8];
		Octree* parent;
	public:
		Octree(int level, Prism const& bounds, Octree* parent);
		void clear();
		void split();
		int getIndex(IPhysicsObject* object) const;
		int getIndex(Ray const& ray, float dist) const;
		void insert(IPhysicsObject* object);
		void retrieveCollisions(vector<IPhysicsObject*>& outList, IPhysicsObject* collider) const;
		void retrieveCollisions(vector<IPhysicsObject*>& outList, Ray const& ray, float dist) const;
		void resetTree(int level, Prism const& bounds);
		void fillTree(vector<IEntity*> const& objects);
		void getChildLeaves(vector<IPhysicsObject*>& outList) const;
		int remove(long ID);
		bool empty() const;
		Prism const& getBounds() const;

		~Octree();
	};
}