#include "Renderable.h"
//Should probably double check this

namespace ginkgo {

	Renderable::Renderable(const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& rot, const glm::vec3& vel, const glm::vec3& accel) :
		Entity::Entity(pos, rot, vel, accel)
	{
		scale = scl;
	}

	const glm::vec3& Renderable::getScale() const
	{
		return scale;
	}
	void Renderable::setScale(const glm::vec3& scl)
	{
		scale = scl;
	}

}
