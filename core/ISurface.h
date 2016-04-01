#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface
	{
	public:
		virtual void getNormal(const Triangle& t, glm::vec3& normOut) const = 0;
		virtual bool intersectsWith(const Triangle& t, const Ray& ray, float distance) const = 0;
		virtual bool doesIntersect(const Ray& ray, float distance) const = 0;
		virtual float getIntersectionValue(const Ray& ray) const = 0;
	};
}