#pragma once

#include "IRenderer.h"



namespace ginkgo
{
	class IPhongShader;
	class ScreenBuffer;
	class IText;
	class ICubeMap;
	class ILayer;

	struct TextLabel
	{
		TextLabel(string const& text, float x, float y, float scale, vec3 const& color)
			: text(text), x(x), y(y), scale(scale), color(color)
		{}

		TextLabel()
			: text(""), x(0), y(0), scale(1), color(vec4())
		{}

		void operator=(const TextLabel& other)
		{
			text = other.text;
			x = other.x;
			y = other.y;
			scale = other.scale;
			color = other.color;
		}

		string text;
		float x, y;
		float scale;
		vec3 color;
	};

	class Renderer : public IRenderer
	{
	private:
		int textCounter;

		map<int, TextLabel> textLabels;
		IPhongShader* lighting;
		ICubeMap* skybox;
		ScreenBuffer* renderSurface;
		IText* textRenderer;
		ILayer* renderLayer;
		IWindow* window;
		ICamera* camera;

	public:
		Renderer(IWindow* window);

		int addRenderable(IRenderable* renderable) override;
		void removeRenderable(int UID) override;
		
		IRenderable* getRenderable(int UID) const override;

		void setDirectionalLight(const DirectionalLight& directionalLight) override;
		void setAmbientLight(const vec4& ambientLight) override;
		int addPointLight(const PointLight& light) override;
		void removePointLight(int index) override;
		void setPointLight(int index, const PointLight& light) override;
		void setPointLightPosition(int index, const vec3& position) override;

		const DirectionalLight& getDirectionalLight() const override;
		const vec4& getAmbientLight() const override;
		const PointLight& getPointLight(int index) const override;

		///Text management
		void createTextComponent(string const& path, int fontSize) override;
		int addText(string const& text, float x, float y, float scale, vec3 const& color) override;
		void setTextPosition(int index, float x, float y) override;
		void removeText(int index) override;
		void editText(int index, string const& text) override;

		///Skybox management
		void loadSkybox(map<unsigned int, string> paths, float scale) override;
		void unloadSkybox() override;

		ICamera* getCamera() override;

		void renderAndSwap() override;
	};
}