#pragma once

#include "ICollisionMesh.h"

namespace ginkgo
{
	class SphereMesh : public ICollisionMesh
	{
	private:
		IPhysicsComponent* owner;
		SphereProperties props;

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
			return CMESH_SHAPE_SPHERE;
		}

		vec3 support(vec3 const& direction) const override
		{
			return vec3();
		}

		void setMeshProperties(MeshProperties const& prop) override
		{
			memcpy(&props, &prop, sizeof(SphereProperties));
		}

		MeshProperties const& getMeshProperties() const override
		{
			return props;
		}
	};

	ICollisionMesh* createSphereCollision(float radius)
	{
		SphereProperties prop;
		prop.radius = radius;
		SphereMesh* m = new SphereMesh();
		m->setMeshProperties(prop);

		return m;
	}
}