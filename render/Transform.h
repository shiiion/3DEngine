#pragma once

#include "ITransform.h"

namespace ginkgo {

	class Transform : public ITransform
	{
	private:
		mutable mat4 matrix;
		mat4 dilation;
		mat4 translation;
		mat4 rotation;
		mat4 identity;
		mutable bool matrixOOD;
	public:
		Transform() : Transform(mat4()) { }
		Transform(const mat4& matrix) { this->matrix = matrix; matrixOOD = true; identity = mat4(); }
		const mat4& getMatrix() const override 
		{
			if (matrixOOD)
			{
				matrixOOD = false;
				matrix = rotation * translation * dilation;
			}
			return matrix; 
		}
		void setMatrix(const mat4& matrix) override { this->matrix = matrix; }
		
		const mat4& scaleMatrix(const vec3& scale) override
		{ 
			dilation = glm::scale(identity, scale);
			matrixOOD = true;
			return matrix;
		}

		const mat4& translateMatrix(const vec3& translation) override
		{ 
			this->translation = translate(identity, translation);
			matrixOOD = true;
			return matrix; 
		}

		const mat4& rotateMatrix(float angleInRadians, const vec3& rotation) override 
		{ 
			this->rotation = rotate(identity, angleInRadians, rotation);
			matrixOOD = true;
			return matrix; 
		}
	};

}

/*
templated in the future for any kind of glm data type as needed
*/