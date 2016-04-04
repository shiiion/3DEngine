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
		float length;
		float width;
		float height;

		ISurface* faces[CMESH_NUM_SURFACES];

		//buffer for functions
		glm::vec3 vertices[CMESH_NUM_VERTICES];

		IPhysicsObject const* owner;

		MoveInfo lastMove;
		
		void createFaces();
	public:
		CollisionMesh(IPhysicsObject const* owner, float l, float w, float h);
		virtual ISurface const* const* getFaces() const override;
		virtual glm::vec3 const* getBoundingVertices() const override;
		virtual MoveInfo const& getLastMove() const override;
		virtual void generateVertexPath(float deltaTime) override;

		virtual void setOwner(IPhysicsObject const* owner) override;
	};
}