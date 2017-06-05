#pragma once

#include "RenderResource.h"

namespace ginkgo
{
	class IRenderable;
	class ICamera;
	class IWindow;
	struct DirectionalLight;
	struct PointLight;

	///wrapper for all renderer classes
	class IRenderer
	{
	public:
		///renderable management
		//returns the UID of this renderable for removal
		virtual int addRenderable(IRenderable* renderable) = 0;
		virtual void removeRenderable(int UID) = 0;
		
		virtual IRenderable* getRenderable(int UID) const = 0;

		///light management
		virtual void setDirectionalLight(const DirectionalLight& directionalLight) = 0;
		virtual void setAmbientLight(const vec4& ambientLight) = 0;
		virtual int addPointLight(const PointLight& light) = 0;
		virtual void removePointLight(int index) = 0;
		virtual void setPointLight(int index, const PointLight& light) = 0;
		virtual void setPointLightPosition(int index, const vec3& position) = 0;

		virtual const DirectionalLight& getDirectionalLight() const = 0;
		virtual const vec4& getAmbientLight() const = 0;
		virtual const PointLight& getPointLight(int index) const = 0;

		///Text management
		virtual void createTextComponent(string const& path, int fontSize) = 0;
		virtual int addText(string const& text, float x, float y, float scale, vec3 const& color) = 0;
		virtual void setTextPosition(int index, float x, float y) = 0;
		virtual void removeText(int index) = 0;
		virtual void editText(int index, string const& text) = 0;

		///Skybox management
		virtual void loadSkybox(map<unsigned int, string> paths, float scale) = 0;
		virtual void unloadSkybox() = 0;

		virtual ICamera* getCamera() = 0;

		virtual void renderAndSwap() = 0;
	};

	//renderer is a singleton, only one can exist 
	DECLSPEC_RENDER IRenderer* initRenderer(IWindow* window);
	DECLSPEC_RENDER IRenderer* getRendererInstance();
}