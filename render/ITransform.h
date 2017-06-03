#pragma once

#include "RenderResource.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ginkgo
{
	class ITransform
	{
	public:
		virtual const mat4& getMatrix() const = 0;
		virtual void setMatrix(const mat4& matrix) = 0;

		virtual const mat4& scaleMatrix(const vec3& scale) = 0;
		virtual const mat4& translateMatrix(const vec3& translation) = 0;
		virtual const mat4& rotateMatrix(float angleInRadians, const vec3& rotation) = 0;
	};

	DECLSPEC_RENDER ITransform* transformFactory();
}