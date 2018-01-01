#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class ISurface
	{
	public:
		virtual void getNormal(const Triangle& t, vec3& normOut) const = 0;
		virtual bool testIntersection(const Triangle& t, const Ray& ray, float distance) const = 0;
		virtual bool intersectsWithSurface(const Ray& ray, float distance) const = 0;
		virtual float getIntersectionValue(const Ray& ray) const = 0;

		virtual void translateSurface(const vec3& translation) = 0;


		virtual ~ISurface() = 0;
	};

	DECLSPEC_CORE ISurface* createSurface(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& v4);
}