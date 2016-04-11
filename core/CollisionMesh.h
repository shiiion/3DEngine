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

		float halfWidth;
		float halfHeight;
		float halfLength;

		glm::vec3 widthAxis;	//(1, 0, 0) without rotation
		glm::vec3 heightAxis;	//(0, 1, 0) without rotation
		glm::vec3 lengthAxis;	//(0, 0, 1) without rotation


		IPhysicsObject const* owner;

		MoveInfo lastMove;

//		void createFaces();
	public:
		CollisionMesh(float l, float w, float h);
		virtual glm::vec3 const* getBoundingVertices() const override;
		virtual MoveInfo const& getLastMove() const override;
		virtual void generateVertexPath(float deltaTime) override;

		virtual void setOwner(IPhysicsObject const* owner) override;

		virtual bool testCollision(ICollisionMesh const& other, float deltaTime) override;
		virtual float getCollisionTime(ICollisionMesh const& other, float deltaTime) const override;
		virtual void finalizeMove() override;

		virtual glm::vec3 const& getWidthAxis() const override;
		virtual glm::vec3 const& getHeightAxis() const override;
		virtual glm::vec3 const& getLengthAxis() const override;

		virtual glm::vec3 const& getCenter() const override;
		virtual float getLengthExtent() const override;
		virtual float getWidthExtent() const override;
		virtual float getHeightExtent() const override;

		virtual void generateCollisionInfo(ICollisionMesh const& other, float intersectTime) override;
		//TRUE if not intersecting, FALSE if intersecting
		bool testAxis(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime);
	};
}