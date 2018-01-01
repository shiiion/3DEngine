#pragma once

#include "RenderResource.h"
#include "LightStructs.h"

namespace ginkgo
{
	struct Material;
	
	class IPhongShader
	{
	public:
		virtual void updateUniforms(const mat4& model, const mat4& transformProjectionViewModel, const Material& material, const vec3& cameraPosition) const = 0;

		virtual const vec4& getAmbientLight() const = 0;
		virtual void setAmbientLight(const vec4& ambientLight) = 0;

		virtual int addPointLight(PointLight const& light) = 0;
		virtual void removePointLight(int index) = 0;
		virtual void setPointLightPosition(int index, const vec3& position) = 0;
		virtual void setPointLight(int index, const PointLight& light) = 0;

		virtual const DirectionalLight& getDirectionalLight() const = 0;
		virtual void setDirectionalLight(const DirectionalLight& directionalLight) = 0;
		virtual const PointLight& getPointLight(int index) = 0;

		virtual void setUniform(const string& name, const DirectionalLight& directionalLight) const = 0;
		virtual void setUniform(const string& name, const BaseLight& baseLight) const = 0;
		virtual void setUniform(const string& name, const PointLight& pointLight) const = 0;

		virtual ~IPhongShader() = 0;
	};

	DECLSPEC_RENDER IPhongShader* phongShaderFactory();
}