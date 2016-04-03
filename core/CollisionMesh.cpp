#include "CollisionMesh.h"
#include "IPhysicsObject.h"
#include "ISurface.h"

namespace ginkgo
{
	CollisionMesh::CollisionMesh(IPhysicsObject const* owner, float l, float w, float h)
	{
		this->owner = owner;
		length = l;
		width = w;
		height = h;

		createFaces();
	}

	void CollisionMesh::createFaces()
	{
		glm::vec3 const& origin = owner->getPosition();

		vertices[0] = origin + glm::vec3(width / 2, height / 2, length / 2);
		vertices[1] = origin + glm::vec3(-width / 2, height / 2, length / 2);
		vertices[2] = origin + glm::vec3(-width / 2, -height / 2, length / 2);
		vertices[3] = origin + glm::vec3(width / 2, -height / 2, length / 2);
		vertices[4] = origin + glm::vec3(width / 2, height / 2, -length / 2);
		vertices[5] = origin + glm::vec3(-width / 2, height / 2, -length / 2);
		vertices[6] = origin + glm::vec3(-width / 2, -height / 2, -length / 2);
		vertices[7] = origin + glm::vec3(width / 2, -height / 2, -length / 2);

		faces[0] = createSurface(vertices[0], vertices[1], vertices[2], vertices[3]);
		faces[1] = createSurface(vertices[4], vertices[0], vertices[3], vertices[7]);
		faces[2] = createSurface(vertices[5], vertices[4], vertices[7], vertices[6]);
		faces[3] = createSurface(vertices[1], vertices[5], vertices[6], vertices[2]);
		faces[4] = createSurface(vertices[4], vertices[5], vertices[1], vertices[0]);
		faces[5] = createSurface(vertices[3], vertices[2], vertices[6], vertices[7]);
	}

	ISurface** CollisionMesh::getFaces() const
	{

	}

	void CollisionMesh::getBoundingVertices(vector<glm::vec3>& vertexList) const
	{

	}

	void CollisionMesh::generateVertexPath(float deltaTime)
	{
		if (owner == nullptr)
			return;
	}

	void CollisionMesh::setOwner(IPhysicsObject const* owner)
	{
		this->owner = owner;
	}
}