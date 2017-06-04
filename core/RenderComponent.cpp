#include "RenderComponent.h"
#include "Core.h"

namespace ginkgo
{
	RenderComponent::RenderComponent(IEntity* parent, IRenderable* mesh) 
		: parent(parent)
	{
		scale = glm::vec3(1, 1, 1);
		this->mesh = mesh;
	}

	const glm::vec3& RenderComponent::getScale() const
	{
		return scale;
	}

	void RenderComponent::setScale(const glm::vec3& scl)
	{
		scale = scl;
	}

	void RenderComponent::onTick(float elapsedTime)
	{

	}

	void RenderComponent::onTickEnd(float elapsedTime)
	{

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

