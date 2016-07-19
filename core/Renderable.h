#pragma once
#include "Entity.h"
#include "IRenderable.h"

namespace ginkgo 
{
	class Renderable : public IRenderable
	{
	private:
		glm::vec3 scale;
		IRenderMesh const* mesh;

		IEntity* const parent;

	public:
		Renderable(IEntity* parent, IRenderMesh const* mesh, const glm::vec3& scl = glm::vec3(1, 1, 1));
		const glm::vec3& getScale() const override;
		void setScale(const glm::vec3& scl) override;
		IRenderMesh const* getRenderMesh() const override;
		void setRenderMesh(IRenderMesh const* mesh) override;
		void render(const glm::vec3& pos) override;

		IEntity* const getParent() const override;
	};
}