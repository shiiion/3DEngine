#include "RenderComponent.h"
#include "Core.h"
#include <IRenderable.h>
#include <ITransform.h>
#include <IRenderer.h>

namespace ginkgo
{
	RenderComponent::RenderComponent(IEntity* parent, IRenderable* mesh) 
		: parent(parent)
	{
		this->mesh = mesh;
		setRotation(parent->getRotation());
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

	void RenderComponent::setRotation(quat const& rotation) 
	{
		mesh->getTransform().rotateMatrix(rotation);
	}

	quat const& RenderComponent::getRotation() const 
	{
		return mesh->getTransform().getRotation();
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

	void RenderComponent::onDetach()
	{
		getRendererInstance()->removeRenderable(mesh->getIndex());
		delete mesh;
	}
}

