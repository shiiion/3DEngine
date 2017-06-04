#include "Layer.h"

#include "Camera.h"
#include "PhongShader.h"
#include "Renderable.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeMap.h"

namespace ginkgo {

	Layer::Layer(const vector<IRenderable*>& renderablesL)
		: renderables(renderablesL)
	{
		if (renderablesL.size())
		{
			return;
		}
		sort(renderables.begin(), renderables.end(), compareRenderables);
	}

	GLuint Layer::determineTextureID(IRenderable* r)
	{
		return (r->getMaterial().texture != nullptr) ?
			r->getMaterial().texture->tid :
			Layer::NO_TEXTURE;
	}

	bool Layer::compareRenderables(IRenderable* r1, IRenderable* r2)
	{
		return determineTextureID(r1) < determineTextureID(r2);
	}

	IRenderable* Layer::alterRenderable(int UID) const
	{
		if (UID < 0)
		{
			return nullptr;
		}

		for (unsigned int i = 0; i < renderables.size(); i++)
		{
			if (renderables[i]->getIndex() == UID)
			{
				return renderables[i];
			}
		}
		return nullptr;
	}

	const IRenderable* Layer::getRenderable(int UID) const
	{
		if (UID < 0)
		{
			return nullptr;
		}

		for (unsigned int i = 0; i < renderables.size(); i++)
		{
			if (renderables[i]->getIndex() == UID)
			{
				return renderables[i];
			}
		}
		return nullptr;
	}

	const mat4& Layer::getModel() const
	{
		return model.getMatrix();
	}

	//speed doesnt matter for this, just re-sort for each new addition
	//if needed, change this to bsearch->insert
	int Layer::addRenderable(IRenderable* renderable)
	{
		renderables.emplace_back(renderable);
		sort(renderables.begin(), renderables.end(), compareRenderables);
		return renderable->getIndex();
	}

	void Layer::removeRenderable(int UID)
	{
		for (unsigned int i = 0; i < renderables.size(); i++)
		{
			if (renderables[i]->getIndex() == UID)
			{
				renderables.erase(renderables.begin() + i);
				return;
			}
		}
	}

	void Layer::draw(const mat4& transformProjectionView, const vec3& cameraPosition, const IPhongShader& phongShaderI, const ICubeMap& cubeMapI) const
	{
		const PhongShader& phongShader = (const PhongShader&)phongShaderI;
		const CubeMap& cubeMap = (const CubeMap&)cubeMapI;
		phongShader.bind();
		if (renderables.size() > 0)
		{
			phongShader.setUniform1i("diffuseTexture", 0); //dependant on phongFragment.fs
			phongShader.setUniform1i("skybox", 1);		   //dependant on phongFragment.fs
			GLuint currentTextureID = 0;

			for (unsigned int i = 0; i < renderables.size(); i++)
			{
				if (determineTextureID(renderables[i]) != currentTextureID)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, determineTextureID(renderables[i]));
					currentTextureID = determineTextureID(renderables[i]);
				}

				phongShader.updateUniforms(
					model.getMatrix() * renderables[i]->getModel(),
					transformProjectionView * model.getMatrix() * renderables[i]->getModel(),
					renderables[i]->getMaterial(),
					cameraPosition);
				if (renderables[i]->getMaterial().refractiveIndex >= 0) { glActiveTexture(GL_TEXTURE1); cubeMap.bindCubeMapTexture(); }
				renderables[i]->draw();
				if (renderables[i]->getMaterial().refractiveIndex >= 0) cubeMap.unbindCubeMapTexture();

				//glActiveTexture(GL_TEXTURE0);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		phongShader.unbind();
	}

	ILayer* layerFactory(const vector<IRenderable*>& renderables)
	{
		return new Layer(renderables);
	}
}
