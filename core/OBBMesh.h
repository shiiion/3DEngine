#pragma once

#include "ICollisionMesh.h"

namespace ginkgo
{
	class OBBMesh : public ICollisionMesh
	{
	private:
		IPhysicsComponent* owner;
		OBBProperties props;

	public:
		void setOwner(IPhysicsComponent* owner) override
		{
			this->owner = owner;
		}

		IPhysicsComponent* getOwner() override
		{
			return owner;
		}

		int getMeshType() const override
		{
			return CMESH_SHAPE_OBB;
		}

		vec3 support(vec3 const& direction) const override
		{
			return vec3();
		}

		void setMeshProperties(MeshProperties const& prop) override
		{
			memcpy(&props, &prop, sizeof(OBBProperties));
		}

		MeshProperties const& getMeshProperties() const override
		{
			return props;
		}
	};

	ICollisionMesh* createOBBCollision(float halfLength, float halfWidth, float halfHeight)
	{
		OBBProperties prop;
		prop.halfLength = halfLength;
		prop.halfWidth = halfWidth;
		prop.halfHeight = halfHeight;
		OBBMesh* m = new OBBMesh();
		m->setMeshProperties(prop);

		return m;
	}
}