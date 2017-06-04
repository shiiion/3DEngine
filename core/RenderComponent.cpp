#include "RenderComponent.h"
#include "Core.h"

namespace ginkgo
{
	RenderComponent::RenderComponent(IEntity* parent, IRenderMesh const* mesh, const glm::vec3& scl) 
		: parent(parent)
	{
		scale = scl;
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

	RenderComponent::~RenderComponent()
	{
	}

	IEntity* RenderComponent::getParent() 
	{
		return parent;
	}

	IRenderComponent* RenderComponentFactory(IEntity* parent, IRenderMesh const* mesh, const glm::vec3& scl)
	{
		return new RenderComponent(parent, mesh, scl);
	}
}

