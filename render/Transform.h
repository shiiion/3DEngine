#pragma once

#include "ITransform.h"
#include <glm/gtx/quaternion.hpp>

namespace ginkgo {

	class Transform : public ITransform
	{
	private:
		mutable mat4 matrix;
		vec3 dilation;
		vec3 translation;
		quat rot;
		mat4 identity;
		mutable bool matrixOOD;
	public:
		Transform()
		{
			rot = glm::angleAxis(0.f, vec3(0, 1, 0));
			dilation = vec3(1, 1, 1);
			translation = vec3();
			matrixOOD = true;
			getMatrix();
		}

		const mat4& getMatrix() const override 
		{
			if (matrixOOD)
			{
				matrixOOD = false;
				matrix = translate(identity, translation) * rotate(identity, glm::angle(rot), glm::axis(rot)) * glm::scale(identity, dilation);
			}
			return matrix; 
		}

		void setMatrix(const mat4& matrix) override { this->matrix = matrix; }
		
		void scaleMatrix(const vec3& scale) override
		{ 
			dilation = scale;
			matrixOOD = true;
		}

		void translateMatrix(const vec3& translation) override
		{ 
			this->translation = translation;
			matrixOOD = true;
		}

		void rotateMatrix(quat const& rotation) override 
		{
			rot = rotation;
			rot.z = -rot.z;
			matrixOOD = true;
		}

		vec3 const& getScale() const override
		{
			return dilation;
		}
		
		vec3 const& getTranslation() const override
		{
			return translation;
		}

		quat const& getRotation() const override
		{
			return rot;
		}
	};

}

/*
templated in the future for any kind of glm data type as needed
*/