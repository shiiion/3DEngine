#pragma once
#include "Entity.h"
#include "IRenderable.h"
//Should probably double check this

namespace ginkgo 
{
	class Renderable : public IRenderable
	{
	private:
		glm::vec3 scale;
		IRenderMesh const* mesh;

	public:
		Renderable(IRenderMesh const* mesh, const glm::vec3& pos, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
		virtual const glm::vec3& getScale() const override;
		virtual void setScale(const glm::vec3& scl) override;

	};
}