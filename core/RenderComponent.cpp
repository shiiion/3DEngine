#include "RenderComponent.h"
#include "Core.h"
#include <IRenderable.h>
#include <ITransform.h>

namespace ginkgo
{
	RenderComponent::RenderComponent(IEntity* parent, IRenderable* mesh) 
		: parent(parent)
	{
		this->mesh = mesh;
	}

	const vec3& RenderComponent::getScale() const
	{
		return mesh->getTransform().getScale();
	}

	void RenderComponent::setScale(const vec3& scl)
	{
		return mesh->getTransform().scaleMatrix(scl);
	}

	const vec3& RenderComponent::getPosition() const 
	{
		return mesh->getTransform().getTranslation();
	}

	void RenderComponent::setPosition(const vec3& pos) 
	{
		mesh->getTransform().translateMatrix(pos);
	}

	void RenderComponent::setRotation(const vec3& axis, float angle) 
	{
		mesh->getTransform().rotateMatrix(angle, axis);
	}

	float RenderComponent::getRotation() const 
	{
		return mesh->getTransform().getAngle();
	}

	const vec3& RenderComponent::getAxis() const 
	{
		return mesh->getTransform().getAxis();
	}

	void RenderComponent::onTick(float elapsedTime)
	{

	}

	void RenderComponent::onTickEnd(float elapsedTime)
	{
		setPosition(parent->getPosition());
	}

	IEntity* RenderComponent::getParent() 
	{
		return parent;
	}

	IRenderComponent* renderComponentFactory(IEntity* parent, IRenderable* mesh)
	{
		return new RenderComponent(parent, mesh);
	}
}

