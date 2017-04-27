#include "Layer.h"

#include "Camera.h"
#include "PhongShader.h"
#include "Renderable.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "CubeMap.h"

namespace ginkgo {

	Layer::Layer(const std::vector<Renderable*>& renderablesL, const glm::mat4& model)
		: renderables(renderablesL), model(model)
	{
		std::sort(renderables.begin(), renderables.end(), compareRenderables);

		GLuint tid = 0;
		unsigned int size = 0;
		for (unsigned int c = 0; c < renderables.size(); c++)
		{
			tid = determineTextureID(renderables[c]);
			size++;
			if ((c == renderables.size() - 1) || (determineTextureID(renderables[c + 1]) != tid))
			{
				sizeTextureIDs.push_back(size);
				size = 0;
			}
		}

	}

	GLuint Layer::determineTextureID(Renderable* r)
	{
		return (r->getMaterial().getTexture() != nullptr) ?
			r->getMaterial().getTexture()->getID() :
			Layer::NO_TEXTURE;
	}

	bool Layer::compareRenderables(Renderable* r1, Renderable* r2)
	{
		return determineTextureID(r1) < determineTextureID(r2);
	}

	Renderable* Layer::alterRenderable(unsigned int index) const
	{
		if (index < 0 || index >= renderables.size())
			return nullptr;

		for (unsigned int i = 0; i < renderables.size(); i++)
			if (renderables[i]->getIndex() == index)
				return renderables[i];

		return nullptr;
	}

	const Renderable* Layer::getRenderable(unsigned int index) const
	{
		if (index < 0 || index >= renderables.size())
			return nullptr;

		for (unsigned int i = 0; i < renderables.size(); i++)
			if (renderables[i]->getIndex() == index)
				return renderables[i];

		return nullptr;
	}

	const glm::mat4& Layer::getModel() const
	{
		return model.getMatrix();
	}

	void Layer::addRenderable(Renderable* renderable)
	{
		int left = 0;
		int right = renderables.size() - 1;

		int middle = 0;
		bool found = false;
		unsigned int value = determineTextureID(renderable);

		while (left <= right)
		{
			middle = (left + right) / 2;
			unsigned int find = determineTextureID(renderables[middle]);
			if (find == value)
			{
				found = true;
				break;
			}
			else if (find > value)
				right = middle - 1;
			else
				left = middle + 1;
		}
		if (found)
		{
			renderables.insert(renderables.begin() + middle, renderable);
			int sum = 0;
			for (int i = 0; i < sizeTextureIDs.size(); i++)
			{
				sum += sizeTextureIDs[i];
				if (sum > middle)
				{
					sizeTextureIDs[i]++;
					break;
				}
			}
		}
		else
		{
			renderables.push_back(renderable);
			sizeTextureIDs.push_back(1);
		}
	}

	void Layer::draw(const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const
	{
		phongShader.bind();
		if (renderables.size() > 0)
		{
			unsigned int b = 0;
			unsigned int i = 0;
			if ((determineTextureID(renderables[0]) == Layer::NO_TEXTURE))
			{
				phongShader.setUniform1i("skybox", 0);		   //dependant on phongFragment.fs
				phongShader.setUniform1i("diffuseTexture", 1); //dependant on phongFragment.fs

				glActiveTexture(GL_TEXTURE0);
				cubeMap.bindCubeMapTexture();
				for (int i = 0; i < sizeTextureIDs[0]; i++)
				{
					phongShader.updateUniforms(
						model.getMatrix() * renderables[i]->getModel(),
						transformProjectionView * model.getMatrix() * renderables[i]->getModel(),
						renderables[i]->getMaterial(),
						cameraPosition);
					renderables[i]->draw();
				}
				cubeMap.unbindCubeMapTexture();

				b = sizeTextureIDs[0];
				i = 1;
			}

			phongShader.setUniform1i("diffuseTexture", 0); //dependant on phongFragment.fs
			phongShader.setUniform1i("skybox", 1);		   //dependant on phongFragment.fs
			for (; i < sizeTextureIDs.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, determineTextureID(renderables[b]));
				for (unsigned int a = 0; a < sizeTextureIDs[i]; a++)
				{
					phongShader.updateUniforms(
						model.getMatrix() * renderables[b]->getModel(),
						transformProjectionView * model.getMatrix() * renderables[b]->getModel(),
						renderables[b]->getMaterial(),
						cameraPosition);
					if (renderables[b]->getMaterial().getRefractiveIndex() >= 0) { glActiveTexture(GL_TEXTURE1); cubeMap.bindCubeMapTexture(); }
					renderables[b]->draw();
					if (renderables[b]->getMaterial().getRefractiveIndex() >= 0) cubeMap.unbindCubeMapTexture();
					b++;
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		phongShader.unbind();
	}

	void Layer::drawButOne(unsigned int index, const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const
	{
		phongShader.bind();
		if (renderables.size() > 0)
		{
			unsigned int b = 0;
			unsigned int i = 0;
			if ((determineTextureID(renderables[0]) == Layer::NO_TEXTURE))
			{
				phongShader.setUniform1i("skybox", 0);		   //dependant on phongFragment.fs
				phongShader.setUniform1i("diffuseTexture", 1); //dependant on phongFragment.fs

				glActiveTexture(GL_TEXTURE0);
				cubeMap.bindCubeMapTexture();
				for (int i = 0; i < sizeTextureIDs[0]; i++)
				{
					if (index != i)
					{
						phongShader.updateUniforms(
							model.getMatrix() * renderables[i]->getModel(),
							transformProjectionView * model.getMatrix() * renderables[i]->getModel(),
							renderables[i]->getMaterial(),
							cameraPosition);
						renderables[i]->draw();
					}
				}
				cubeMap.unbindCubeMapTexture();

				b = sizeTextureIDs[0];
				i = 1;
			}

			phongShader.setUniform1i("diffuseTexture", 0); //dependant on phongFragment.fs
			phongShader.setUniform1i("skybox", 1);		   //dependant on phongFragment.fs
			for (; i < sizeTextureIDs.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, determineTextureID(renderables[b]));
				for (unsigned int a = 0; a < sizeTextureIDs[i]; a++)
				{
					if (index != b)
					{
						phongShader.updateUniforms(
							model.getMatrix() * renderables[b]->getModel(),
							transformProjectionView * model.getMatrix() * renderables[b]->getModel(),
							renderables[b]->getMaterial(),
							cameraPosition);
						if (renderables[b]->getMaterial().getRefractiveIndex() >= 0) { glActiveTexture(GL_TEXTURE1); cubeMap.bindCubeMapTexture(); }
						renderables[b]->draw();
						if (renderables[b]->getMaterial().getRefractiveIndex() >= 0) cubeMap.unbindCubeMapTexture();
					}
					b++;
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		phongShader.unbind();
	}

	void Layer::drawOne(unsigned int index, const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const
	{
		phongShader.bind();

		if ((determineTextureID(renderables[index]) == Layer::NO_TEXTURE))
		{
			phongShader.setUniform1i("skybox", 0);		   //dependant on phongFragment.fs
			phongShader.setUniform1i("diffuseTexture", 1); //dependant on phongFragment.fs

			glActiveTexture(GL_TEXTURE0);
			cubeMap.bindCubeMapTexture();

			phongShader.updateUniforms(
				model.getMatrix() * renderables[index]->getModel(),
				transformProjectionView * model.getMatrix() * renderables[index]->getModel(),
				renderables[index]->getMaterial(),
				cameraPosition);
			renderables[index]->draw();
			cubeMap.unbindCubeMapTexture();
		}
		else
		{
			phongShader.setUniform1i("diffuseTexture", 0); //dependant on phongFragment.fs
			phongShader.setUniform1i("skybox", 1);		   //dependant on phongFragment.fs

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, determineTextureID(renderables[index]));
			phongShader.updateUniforms(
				model.getMatrix() * renderables[index]->getModel(),
				transformProjectionView * model.getMatrix() * renderables[index]->getModel(),
				renderables[index]->getMaterial(),
				cameraPosition);
			if (renderables[index]->getMaterial().getRefractiveIndex() >= 0) { glActiveTexture(GL_TEXTURE1); cubeMap.bindCubeMapTexture(); }
			renderables[index]->draw();
			if (renderables[index]->getMaterial().getRefractiveIndex() >= 0) cubeMap.unbindCubeMapTexture();
			
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		phongShader.unbind();
	}
}
