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

		virtual void scaleMatrix(const vec3& scale) = 0;
		virtual void translateMatrix(const vec3& translation) = 0;
		virtual void rotateMatrix(float angleInRadians, const vec3& rotation) = 0;
		
		virtual vec3 const& getScale() const = 0;
		virtual vec3 const& getTranslation() const = 0;
		virtual vec3 const& getAxis() const = 0;
		virtual float const& getAngle() const = 0;
	};

	DECLSPEC_RENDER ITransform* transformFactory();
}