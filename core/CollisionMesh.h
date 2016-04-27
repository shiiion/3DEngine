#pragma once

#include "ICollisionMesh.h"

/*
all collision meshes are rotated prisms
the physics object is located at the center of the collision mesh, therefore
bounds are at 

+-(l/2)
+-(w/2)
+-(h/2)

		  ________
		 /|      /|
		/_|_____/ |    h
		| |_____|_|
		| /     | /
		|/______|/    l
    
			w
*/


namespace ginkgo
{
	class CollisionMesh : public ICollisionMesh
	{
	private:
		glm::vec3 center;

		

		//width, height, length
		float extents[3];
		glm::vec3 axes[3];

		bool newSeparatingAxis;

		glm::vec3 lastSeparatingAxis;

		IPhysicsObject const* owner;

		MoveInfo lastMove;

		void getLastSeparatingAxis(ICollisionMesh const& other, float deltaTime);
	public:
		CollisionMesh(float l, float w, float h);
		virtual glm::vec3 const* getBoundingVertices() const override;
		virtual MoveInfo const& getLastMove() const override;
		virtual void generateVertexPath(float deltaTime) override;

		virtual void setOwner(IPhysicsObject const* owner) override;

		virtual bool testCollision(ICollisionMesh const& other, float deltaTime) override;
		virtual float getCollisionTime(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime) const override;
		virtual void finalizeMove() override;

		virtual glm::vec3 const& getAxis(int axis) const override;

		virtual glm::vec3 const& getCenter() const override;
		virtual float getExtent(int extent) const override;

		virtual void generateCollisionInfo(ICollisionMesh const& other, float intersectTime) override;
		//TRUE if not intersecting, FALSE if intersecting
		bool testAxis(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime);
	};
}