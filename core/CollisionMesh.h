#pragma once

#include "ICollisionMesh.h"

#define AXIS_THIS_1 1
#define AXIS_THIS_2 2
#define AXIS_THIS_3 3
#define AXIS_OTHER_1 4
#define AXIS_OTHER_2 5
#define AXIS_OTHER_3 6
#define AXIS_1X1 7
#define AXIS_1X2 8
#define AXIS_1X3 9
#define AXIS_2X1 10
#define AXIS_2X2 11
#define AXIS_2X3 12
#define AXIS_3X1 13
#define AXIS_3X2 14
#define AXIS_3X3 15
#define AXIS_CROSS(a,b) (AXIS_1X1 + ((int)(b)) + (3 * ((int)(a))))

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

		int lastSeparatingAxisType;
		int intersectSide;

		//width (x), height(y), length(z)
		float extents[3];
		glm::vec3 axes[3];

		bool newSeparatingAxis;

		glm::vec3 lastSeparatingAxis;

		IPhysicsObject const* owner;

		MoveInfo lastMove;

		void getLastSeparatingAxis(ICollisionMesh const& other, float deltaTime);
		void this_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut);
		void other_lastSeparatingAxis(ICollisionMesh const& other, float intersectTime, CollisionInfo& collisionOut);
	public:
		CollisionMesh(float w, float h, float l, glm::vec3 const& wAxis, glm::vec3 const& hAxis, glm::vec3 const& lAxis);
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

		virtual CollisionInfo generateCollisionInfo(ICollisionMesh const& other, float intersectTime) override;
		//TRUE if not intersecting, FALSE if intersecting
		bool testAxis(glm::vec3 const& axisNorm, ICollisionMesh const& other, float deltaTime);
	};
}