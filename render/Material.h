#pragma once

#include "RenderResource.h"

namespace ginkgo {

	struct Texture;

	struct Material
	{
		const Texture* texture;
		vec4 color;
		float specularIntensity;
		float specularPower;
		float refractiveIndex; //	>= 1.0f -> depends on phongFragment.fs
		float rIntensity; // 0 <= x <= 1

	public:
		Material(float refractiveIndex)
			: Material(2, 32, refractiveIndex, 1.0f, vec4(), nullptr)
		{ }

		Material(const Texture* texture)
			: Material(2, 32, -1, 1.0f, vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{ }

		Material(const vec4& color, const Texture* texture)
			: Material(2, 32, -1, 1.0f, color, texture)
		{ }

		Material(float refractiveIndex, const Texture* texture)
			: Material(2, 32, refractiveIndex, 1.0f, vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{ }

		Material(float refractiveIndex, float rIntensity, const Texture* texture)
			: Material(2, 32, refractiveIndex, rIntensity, vec4(1.0f, 1.0f, 1.0f, 1.0f), texture)
		{}

		Material(float refractiveIndex, float rIntensity, const vec4& color, const Texture* texture)
			: Material(2, 32, refractiveIndex, rIntensity, color, texture)
		{ }

		Material(float refractiveIndex, const vec4& color, const Texture* texture)
			: Material(2, 32, refractiveIndex, 1.0f, color, texture)
		{ }

		Material(float specularIntensity, float specularExponent, float refractiveIndex, float rIntensity, const vec4& color, const Texture* texture)
			: specularIntensity(specularIntensity), specularPower(specularExponent), refractiveIndex(refractiveIndex), rIntensity(rIntensity), color(color), texture(texture)
		{ }
	};

}
