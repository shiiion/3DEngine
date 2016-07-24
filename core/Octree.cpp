
#include "Octree.h"
#include "IPhysicsObject.h"
#include "CollisionMesh.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

namespace ginkgo
{
	float getTop(CollisionMesh const& mesh);
	float getBot(CollisionMesh const& mesh);
	float getLeft(CollisionMesh const& mesh);
	float getRight(CollisionMesh const& mesh);
	float getFront(CollisionMesh const& mesh);
	float getBack(CollisionMesh const& mesh);


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

	void Octree::clear()
	{
		objects.clear();
		for (int a = 0; a < 8; a++)
		{
			if (leaves[a] != nullptr)
			{
				delete leaves[a];
				leaves[a] = nullptr;
			}
		}
	}

	void Octree::split()
	{
		float w = bounds.w / 2.f;
		float h = bounds.h / 2.f;
		float l = bounds.l / 2.f;

		if (leaves[0] == nullptr)
			leaves[0] = new Octree(level + 1, Prism(bounds.x, bounds.y, bounds.z, w, h, l), this);
		if (leaves[1] == nullptr)
			leaves[1] = new Octree(level + 1, Prism(bounds.x, bounds.y, bounds.z + l, w, h, l), this);
		if (leaves[2] == nullptr)
			leaves[2] = new Octree(level + 1, Prism(bounds.x, bounds.y + h, bounds.z + l, w, h, l), this);
		if (leaves[3] == nullptr)
			leaves[3] = new Octree(level + 1, Prism(bounds.x, bounds.y + h, bounds.z, w, h, l), this);
		if (leaves[4] == nullptr)
			leaves[4] = new Octree(level + 1, Prism(bounds.x + w, bounds.y, bounds.z, w, h, l), this);
		if (leaves[5] == nullptr)
			leaves[5] = new Octree(level + 1, Prism(bounds.x + w, bounds.y, bounds.z + l, w, h, l), this);
		if (leaves[6] == nullptr)
			leaves[6] = new Octree(level + 1, Prism(bounds.x + w, bounds.y + h, bounds.z + l, w, h, l), this);
		if (leaves[7] == nullptr)
			leaves[7] = new Octree(level + 1, Prism(bounds.x + w, bounds.y + h, bounds.z, w, h, l), this);
	}

	int Octree::getIndex(IPhysicsObject* object) const
	{
		int index = -1;
		float hfWidth = bounds.x + bounds.w / 2.f;
		float hfHeight = bounds.y + bounds.h / 2.f;
		float hfLength = bounds.z + bounds.l / 2.f;


		ICollisionMesh* mesh = object->getCollisionMesh();

		float top, bot, left, right, front, back;

		if (mesh->getCollisionShape() == CMESH_SHAPE_OBB)
		{
			top = getTop(*(CollisionMesh*)mesh);
			bot = getBot(*(CollisionMesh*)mesh);
			left = getLeft(*(CollisionMesh*)mesh);
			right = getRight(*(CollisionMesh*)mesh);
			front = getFront(*(CollisionMesh*)mesh);
			back = getBack(*(CollisionMesh*)mesh);
		}
		else
		{
			//IMPLEMENTME
		}

		bool topHalf = (bot > hfHeight);
		bool botHalf = (top < hfHeight);

		bool leftHalf = (right < hfWidth);
		bool rightHalf = (left > hfWidth);

		if (back > hfLength)//z+
		{
			if (topHalf)//y+
			{
				if (leftHalf)//x-
				{
					index = 2;
				}
				else if (rightHalf)//x+
				{
					index = 6;
				}
			}
			else if (botHalf)//y-
			{
				if (leftHalf)//x-
				{
					index = 1;
				}
				else if (rightHalf)//x+
				{
					index = 5;
				}
			}
		}
		else if (front < hfLength)//z-
		{
			if (topHalf)//y+
			{
				if (leftHalf)//x-
				{
					index = 3;
				}
				else if (rightHalf)//x+
				{
					index = 7;
				}
			}
			else if (botHalf)//y-
			{
				if (leftHalf)//x-
				{
					index = 0;
				}
				else if (rightHalf)//x+
				{
					index = 4;
				}
			}
		}
		return index;
	}

	int Octree::getIndex(Ray const& ray, float dist) const
	{
		int index = -1;
		float hfWidth = bounds.x + bounds.w / 2.f;
		float hfHeight = bounds.y + bounds.h / 2.f;
		float hfLength = bounds.z + bounds.l / 2.f;

		glm::vec3 endpt = ray.point + (ray.direction * dist);

		float top = max(endpt.y, ray.point.y);
		float bot = min(endpt.y, ray.point.y);
		float left = min(endpt.x, ray.point.x);
		float right = max(endpt.x, ray.point.x);
		float front = max(endpt.z, ray.point.z);
		float back = min(endpt.z, ray.point.z);

		bool topHalf = (bot > hfHeight);
		bool botHalf = (top < hfHeight);

		bool leftHalf = (right < hfWidth);
		bool rightHalf = (left > hfWidth);

		//TODO: repeated code, refactor me

		if (back > hfLength)//z+
		{
			if (topHalf)//y+
			{
				if (leftHalf)//x-
				{
					index = 2;
				}
				else if (rightHalf)//x+
				{
					index = 6;
				}
			}
			else if (botHalf)//y-
			{
				if (leftHalf)//x-
				{
					index = 1;
				}
				else if (rightHalf)//x+
				{
					index = 5;
				}
			}
		}
		else if (front < hfLength)//z-
		{
			if (topHalf)//y+
			{
				if (leftHalf)//x-
				{
					index = 3;
				}
				else if (rightHalf)//x+
				{
					index = 7;
				}
			}
			else if (botHalf)//y-
			{
				if (leftHalf)//x-
				{
					index = 0;
				}
				else if (rightHalf)//x+
				{
					index = 4;
				}
			}
		}
		return index;
	}

	void Octree::reinsert(IEntity* entity)
	{
		Octree* insertTree = nullptr;
		if (!shouldMove(entity->getPhysics(), &insertTree))
		{
			if (insertTree != nullptr)
			{
				remove(entity->getEntityID());
				insertTree->insert(entity->getPhysics());
			}
		}
	}

	bool Octree::shouldMove(IPhysicsObject* object, Octree** cachedFind)
	{
		int index = getIndex(object);
		if (index != -1)
		{
			if (leaves[index] == nullptr)
			{
				return false;
			}
			return leaves[index]->shouldMove(object, cachedFind);
		}
		bool found = false;
		for (IPhysicsObject* o : objects)
		{
			if (object->getParent()->getEntityID() == o->getParent()->getEntityID())
			{
				return true;
			}
		}
		(*cachedFind) = this;
		return false;
	}

	void Octree::insert(IPhysicsObject* object)
	{
		if (leaves[0] != nullptr)
		{
			int index = getIndex(object);
			if (index != -1)
			{
				leaves[index]->insert(object);
				return;
			}
		}

		objects.emplace_back(object);
		
		if (objects.size() > OCTREE_MAXENTS && level < OCTREE_MAXLEVELS)
		{
			if (leaves[0] == nullptr)
			{
				split();
			}

			UINT32 a = 0;
			while (a < objects.size())
			{
				int index = getIndex(objects[a]);
				if (index != -1)
				{
					leaves[index]->insert(objects[a]);
					objects.erase(objects.begin() + a);
				}
				else
				{
					a++;
				}
			}
		}
	}

	void Octree::retrieveCollisions(vector<IPhysicsObject*>& outObjects, IPhysicsObject* collider) const
	{
		int index = getIndex(collider);
		if (index != -1 && leaves[0] != nullptr)
		{
			leaves[index]->retrieveCollisions(outObjects, collider);
		}

		for (UINT32 a = 0; a < objects.size(); a++)
		{
			outObjects.emplace_back(objects[a]);
		}
	}

	void Octree::retrieveCollisions(vector<IPhysicsObject*>& outList, Ray const& ray, float dist) const
	{
		int index = getIndex(ray, dist);
		if (index != -1 && leaves[0] != nullptr)
		{
			leaves[index]->retrieveCollisions(outList, ray, dist);
		}

		for (UINT32 a = 0; a < objects.size(); a++)
		{
			outList.push_back(objects[a]);
		}
	}

	void Octree::resetTree(int level, Prism const& bounds)
	{
		if (parent != nullptr)
		{
			parent->resetTree(level, bounds);
			return;
		}

		clear();
		this->level = level;
		this->bounds = bounds;
		objects.reserve(OCTREE_MAXENTS + 1);
		parent = nullptr;
	}

	void Octree::fillTree(vector<IEntity*> const& objects)
	{
		for (IEntity* object : objects)
		{
			if (object->getEntityType() >= physicsObject)
			{
				insert(object->getPhysics());
			}
		}
	}

	int Octree::remove(long ID)
	{
		bool thisTree = false;
		UINT32 index;
		for (index = 0; index < objects.size(); index++)
		{
			if (objects[index]->getParent()->getEntityID() == ID)
			{
				thisTree = true;
				break;
			}
		}

		if (thisTree)
		{
			objects.erase(objects.begin() + index);
			if (objects.empty())
			{
				return REMOVE_FOUNDEMPTY;
			}
			return REMOVE_FOUNDNOTEMPTY;
		}
		else
		{
			for (int a = 0; a < 8; a++)
			{
				if (leaves[a] != nullptr)
				{
					int ret = leaves[a]->remove(ID);
					
				}
			}
		}
		return REMOVE_NOTFOUND;
	}

	bool Octree::empty() const
	{
		return objects.empty();
	}

	void Octree::getChildLeaves(vector<IPhysicsObject*>& outList) const
	{
		for (int a = 0; a < 8; a++)
		{
			if (leaves[a] != nullptr)
			{
				leaves[a]->getChildLeaves(outList);
			}
		}

		for (UINT32 a = 0; a < objects.size(); a++)
		{
			outList.push_back(objects[a]);
		}
	}

	Prism const& Octree::getBounds() const
	{
		return bounds;
	}

	Octree::~Octree()
	{
		clear();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//y+
	float getTop(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xY, yY, zY;
		xY = max(xAxis.y * mesh.getExtent(0), xAxis.y * -mesh.getExtent(0)) + mesh.getCachedCenter().y;
		yY = max(yAxis.y * mesh.getExtent(1), yAxis.y * -mesh.getExtent(1)) + mesh.getCachedCenter().y;
		zY = max(zAxis.y * mesh.getExtent(2), zAxis.y * -mesh.getExtent(2)) + mesh.getCachedCenter().y;

		return max(xY, max(yY, zY));
	}

	//y-
	float getBot(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xY, yY, zY;
		xY = min(xAxis.y * mesh.getExtent(0), xAxis.y * -mesh.getExtent(0)) + mesh.getCachedCenter().y;
		yY = min(yAxis.y * mesh.getExtent(1), yAxis.y * -mesh.getExtent(1)) + mesh.getCachedCenter().y;
		zY = min(zAxis.y * mesh.getExtent(2), zAxis.y * -mesh.getExtent(2)) + mesh.getCachedCenter().y;

		return min(xY, min(yY, zY));
	}

	//x-
	float getLeft(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xX, yX, zX;
		xX = min(xAxis.x * mesh.getExtent(0), xAxis.x * -mesh.getExtent(0)) + mesh.getCachedCenter().x;
		yX = min(yAxis.x * mesh.getExtent(1), yAxis.x * -mesh.getExtent(1)) + mesh.getCachedCenter().x;
		zX = min(zAxis.x * mesh.getExtent(2), zAxis.x * -mesh.getExtent(2)) + mesh.getCachedCenter().x;

		return min(xX, min(yX, zX));
	}

	//x+
	float getRight(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xX, yX, zX;
		xX = max(xAxis.x * mesh.getExtent(0), xAxis.x * -mesh.getExtent(0)) + mesh.getCachedCenter().x;
		yX = max(yAxis.x * mesh.getExtent(1), yAxis.x * -mesh.getExtent(1)) + mesh.getCachedCenter().x;
		zX = max(zAxis.x * mesh.getExtent(2), zAxis.x * -mesh.getExtent(2)) + mesh.getCachedCenter().x;

		return max(xX, max(yX, zX));
	}

	//z+
	float getFront(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xZ, yZ, zZ;
		xZ = max(xAxis.z * mesh.getExtent(0), xAxis.z * -mesh.getExtent(0)) + mesh.getCachedCenter().z;
		yZ = max(yAxis.z * mesh.getExtent(1), yAxis.z * -mesh.getExtent(1)) + mesh.getCachedCenter().z;
		zZ = max(zAxis.z * mesh.getExtent(2), zAxis.z * -mesh.getExtent(2)) + mesh.getCachedCenter().z;

		return max(xZ, max(yZ, zZ));
	}

	//z-
	float getBack(CollisionMesh const& mesh)
	{
		glm::vec3 const& xAxis = mesh.getAxis(0);
		glm::vec3 const& yAxis = mesh.getAxis(1);
		glm::vec3 const& zAxis = mesh.getAxis(2);
		float xZ, yZ, zZ;
		xZ = min(xAxis.z * mesh.getExtent(0), xAxis.z * -mesh.getExtent(0)) + mesh.getCachedCenter().z;
		yZ = min(yAxis.z * mesh.getExtent(1), yAxis.z * -mesh.getExtent(1)) + mesh.getCachedCenter().z;
		zZ = min(zAxis.z * mesh.getExtent(2), zAxis.z * -mesh.getExtent(2)) + mesh.getCachedCenter().z;

		return min(xZ, min(yZ, zZ));
	}
}