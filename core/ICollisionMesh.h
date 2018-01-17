#pragma once

#include "CoreReource.h"


namespace ginkgo
{

#define CMESH_SHAPE_OBB 1
#define CMESH_SHAPE_CAPSULE 2
#define CMESH_SHAPE_SPHERE 3

	class IPhysicsComponent;

	// Make subclass of me for each shape
	struct MeshProperties
	{
		MeshProperties() : scale(vec3()) {}

		vec3 scale;
	};

	struct OBBProperties : public MeshProperties
	{
	public:
		float halfWidth;
		float halfLength;
		float halfHeight;
	};

	struct SphereProperties : public MeshProperties
	{
	public:
		float radius;
	};

	struct CapsuleProperties : public MeshProperties
	{
	public:
		float length;
		float radius;
	};



	// Reduced the size a lot!!!!
	class ICollisionMesh
	{
	public:

		virtual void setOwner(IPhysicsComponent* owner) = 0;
		virtual IPhysicsComponent* getOwner() = 0;

		virtual vec3 support(vec3 const& direction) const = 0;

		virtual int getMeshType() const = 0;

		// Subclasses of ICollisionMesh must also make subclass of MeshProperties
		virtual void setMeshProperties(MeshProperties const& prop) = 0;
		virtual MeshProperties const& getMeshProperties() const = 0;

		virtual ~ICollisionMesh() = 0;
	};

	DECLSPEC_CORE ICollisionMesh* createSphereCollision(float radius);
	// length is of the internal cylinder, excluding hemispheres
	DECLSPEC_CORE ICollisionMesh* createCapsuleCollision(float length, float radius);
	// length follows z, width follows x, height follows y
	DECLSPEC_CORE ICollisionMesh* createOBBCollision(float halfLength, float halfWidth, float halfHeight);
}