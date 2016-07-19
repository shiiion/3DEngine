#include "Renderable.h"
#include "Core.h"

namespace ginkgo
{
	Renderable::Renderable(IEntity* parent, IRenderMesh const* mesh, const glm::vec3& scl) 
		: parent(parent)
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

	void Renderable::render(const glm::vec3& pos)
	{

	}

	IEntity* const Renderable::getParent() const
	{
		return parent;
	}

	IRenderable* renderableFactory(IEntity* parent, IRenderMesh const* mesh, const glm::vec3& scl)
	{
		return new Renderable(parent, mesh, scl);
	}
}

