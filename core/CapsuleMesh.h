#pragma once

#include "ICollisionMesh.h"

namespace ginkgo
{
	class CapsuleMesh : public ICollisionMesh
	{
	private:
		IPhysicsComponent* owner;
		CapsuleProperties props;

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
			return CMESH_SHAPE_CAPSULE;
		}

		vec3 support(vec3 const& direction) const override
		{
			return vec3();
		}

		void setMeshProperties(MeshProperties const& prop) override
		{
			memcpy(&props, &prop, sizeof(CapsuleProperties));
		}

		MeshProperties const& getMeshProperties() const override
		{
			return props;
		}
	};

	ICollisionMesh* createCapsuleCollision(float length, float radius)
	{
		CapsuleProperties prop;
		prop.length = length;
		prop.radius = radius;
		CapsuleMesh* m = new CapsuleMesh();
		m->setMeshProperties(prop);

		return m;
	}
}