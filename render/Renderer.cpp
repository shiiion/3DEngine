#include "Renderer.h"

#include "IPhongShader.h"
#include "ScreenBuffer.h"
#include "ICubeMap.h"
#include "IText.h"
#include "IWindow.h"
#include "ILayer.h"
#include "ICamera.h"

namespace ginkgo
{
	Renderer* primaryRenderer = nullptr;

	Renderer::Renderer(IWindow* window)
	{
		skybox = nullptr;
		textRenderer = nullptr;
		this->window = window;
		textCounter = 0;
		camera = cameraFactory(window);
		renderLayer = layerFactory();
		lighting = phongShaderFactory();
		renderSurface = new ScreenBuffer(window->getWidth(), window->getHeight(), glm::vec4(0, 0, 0, 0), false, false);
	}

	int Renderer::addRenderable(IRenderable* renderable)
	{
		return renderLayer->addRenderable(renderable);
	}

	void Renderer::removeRenderable(int UID)
	{
		renderLayer->removeRenderable(UID);
	}

	IRenderable* Renderer::getRenderable(int UID) const
	{
		return renderLayer->alterRenderable(UID);
	}

	void Renderer::setDirectionalLight(const DirectionalLight& directionalLight)
	{
		lighting->setDirectionalLight(directionalLight);
	}

	void Renderer::setAmbientLight(const vec4& ambientLight)
	{
		lighting->setAmbientLight(ambientLight);
	}

	int Renderer::addPointLight(const PointLight& light)
	{
		return lighting->addPointLight(light);
	}

	void Renderer::removePointLight(int index)
	{
		lighting->removePointLight(index);
	}

	void Renderer::setPointLight(int index, const PointLight& light)
	{
		lighting->setPointLight(index, light);
	}

	void Renderer::setPointLightPosition(int index, const vec3& position)
	{
		lighting->setPointLightPosition(index, position);
	}

	const DirectionalLight& Renderer::getDirectionalLight() const
	{
		return lighting->getDirectionalLight();
	}

	const vec4& Renderer::getAmbientLight() const
	{
		return lighting->getAmbientLight();
	}

	const PointLight& Renderer::getPointLight(int index) const
	{
		return lighting->getPointLight(index);
	}

	void Renderer::createTextComponent(string const& path, int fontSize)
	{
		textRenderer = textRenderFactory(window->getWidth(), window->getHeight(), path.c_str(), fontSize);
	}

	int Renderer::addText(string const& text, float x, float y, float scale, vec3 const& color)
	{
		textLabels[textCounter] = TextLabel(text, x, y, scale, color);
		return textCounter++;
	}

	void Renderer::setTextPosition(int index, float x, float y)
	{
		if (textLabels.find(index) != textLabels.end())
		{
			textLabels[index].x = x;
			textLabels[index].y = y;
		}
	}

	void Renderer::removeText(int index)
	{
		if (textLabels.find(index) == textLabels.end())
		{
			return;
		}
		textLabels.erase(index);
	}

	void Renderer::editText(int index, string const& text)
	{
		if (textLabels.find(index) == textLabels.end())
		{
			return;
		}

		textLabels[index].text = text;
	}

	void Renderer::loadSkybox(map<unsigned int, string> paths, float scale)
	{
		skybox = cubeMapFactory(paths, scale);
	}

	void Renderer::unloadSkybox()
	{
		if (skybox != nullptr)
		{
			delete skybox;
			skybox = nullptr;
		}
	}

	ICamera* Renderer::getCamera()
	{
		return camera;
	}

	void Renderer::renderAndSwap()
	{
		mat4 tPVC = camera->getProjection() * camera->getView() * camera->getCameraPositionTranslation();

		//ScreenBuffer::initalize();
		renderSurface->drawToTexture();
		renderLayer->draw(tPVC, camera->getCameraPosition(), *lighting, *skybox);

		if (skybox != nullptr)
		{
			skybox->draw(tPVC);
		}

		renderSurface->drawToScreen();
		if (textRenderer != nullptr)
		{
			for (auto pair : textLabels)
			{
				const TextLabel& label = pair.second;
				textRenderer->draw(label.text, label.x, label.y, label.scale, label.color);
			}
		}

		window->update();
	}

	IRenderer* initRenderer(IWindow* window)
	{ 
		if (primaryRenderer == nullptr)
		{
			primaryRenderer = new Renderer(window);
		}
		return primaryRenderer;
	}

	IRenderer* getRendererInstance()
	{
		return primaryRenderer;
	}

	IRenderer::~IRenderer() {}
}