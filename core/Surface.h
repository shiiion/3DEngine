#pragma once

#include "ISurface.h"

namespace ginkgo
{
	class Surface : public ISurface
	{
	private:
		Triangle t1;
		Triangle t2;
		void makeTwoTriangles(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4);
	public:
		Surface(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4);

		virtual void getNormal(const Triangle& t, glm::vec3& normOut) const override;
		virtual bool intersectsWith(const Triangle& t, const Ray& ray, float distance) const override;
		virtual bool doesIntersect(const Ray& ray, float distance) const override;
		virtual float getIntersectionValue(const Ray& ray) const override;
	};
}