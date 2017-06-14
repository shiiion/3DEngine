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
		virtual void rotateMatrix(quat const& rotation) = 0;
		
		virtual vec3 const& getScale() const = 0;
		virtual vec3 const& getTranslation() const = 0;
		virtual quat const& getRotation() const = 0;
	};

	DECLSPEC_RENDER ITransform* transformFactory();
}