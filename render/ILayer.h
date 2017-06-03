#pragma once

#include "RenderResource.h"

namespace ginkgo
{
	class ITransform;
	class IPhongShader;
	class ICubeMap;
	class IRenderable;

	class ILayer
	{
	public:
		virtual unsigned int getSize() const = 0;

		virtual int addRenderable(IRenderable* renderable) = 0;
		virtual void removeRenderable(int UID) = 0;
		virtual IRenderable* alterRenderable(int UID) const = 0;
		virtual const IRenderable* getRenderable(int UID) const = 0;

		virtual const mat4& getModel() const = 0;
		virtual ITransform& alterModel() = 0;

		virtual void draw(const mat4& transformProjectionView, const vec3& cameraPosition, const IPhongShader& phongShader, const ICubeMap& cubeMap) const = 0;

	};

	DECLSPEC_RENDER ILayer* layerFactory(const vector<IRenderable*>& renderables = vector<IRenderable*>(), const mat4& model = mat4());

}