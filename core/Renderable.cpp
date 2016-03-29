#include "Renderable.h"
//Should probably double check this

namespace ginkgo 
{
	Renderable::Renderable(IRenderMesh const* mesh, const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		scale = scl;
		this->mesh = mesh;
	}

	const glm::vec3& Renderable::getScale() const
	{
		return scale;
	}

	void Renderable::setScale(const glm::vec3& scl)
	{
		scale = scl;
	}

	IRenderMesh const* Renderable::getRenderMesh() const
	{
		return mesh;
	}

	void Renderable::setRenderMesh(IRenderMesh const* mesh)
	{
		this->mesh = mesh;
	}

	const glm::vec3& Entity::getAcceleration() const
	{
		return acceleration;
	}

	const glm::vec3& Entity::getVelocity() const
	{
		return velocity;
	}

	const glm::vec3& Entity::getRotation() const
	{
		return rotation;
	}

	const glm::vec3& Entity::getPosition() const
	{
		return position;
	}

	void Entity::setAcceleration(const glm::vec3& accel)
	{
		acceleration = accel;
	}

	void Entity::setVelocity(const glm::vec3& vel)
	{
		velocity = vel;
	}

	void Entity::setRotation(const glm::vec3& rot)
	{
		rotation = rot;
	}

	void Entity::setPosition(const glm::vec3& pos)
	{
		position = pos;
	}

	void Entity::tick(float elapsedTime)
	{	
		velocity += acceleration * elapsedTime;
		position += velocity * elapsedTime;
	}

	IRenderable* renderableFactory(IRenderMesh const* mesh, const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel)
	{
		return new Renderable(mesh, pos, scl, rot, vel, accel);
	}
}
