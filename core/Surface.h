#pragma once

#include "ISurface.h"

namespace ginkgo
{
	class Surface : public ISurface
	{
	private:
		Triangle t1;
		Triangle t2;
	public:
		Surface(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4);
		Surface();

		void makeTwoTriangles(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4);
		virtual void getNormal(const Triangle& t, vec3& normOut) const override;
		virtual bool testIntersection(const Triangle& t, const Ray& ray, float distance) const override;
		virtual bool intersectsWithSurface(const Ray& ray, float distance) const override;
		virtual float getIntersectionValue(const Ray& ray) const override;
		virtual void translateSurface(const vec3& translation) override;
	};
}