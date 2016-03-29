#pragma once
#include "Entity.h"
//Should probably double check this

namespace ginkgo 
{
	class Renderable : public Entity
	{
	private:
		glm::vec3 scale;

	public:
		Renderable(const glm::vec3& pos, const glm::vec3& scl = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());

		virtual const glm::vec3& getScale() const;

		virtual void setScale(const glm::vec3& scl);


	};
}