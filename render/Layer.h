#pragma once

#include <algorithm>
#include <vector>

#include "ILayer.h"
#include "Transform.h"

namespace ginkgo {

	class Renderable;
	class PhongShader;
	class Camera;
	class CubeMap;

	class Layer : public ILayer
	{
	private:
		std::vector<IRenderable*> renderables;
		std::vector<unsigned int> sizeTextureIDs;
		Transform model;
		static const unsigned int NO_TEXTURE = 0; //must be equal than 0 -> created textures will never have an id of 0

		static bool compareRenderables(IRenderable* r1, IRenderable* r2);
		static unsigned int determineTextureID(IRenderable* r1);
	public:
		Layer(const std::vector<IRenderable*>& renderables, const glm::mat4& model = glm::mat4());

		unsigned int getSize() const { return renderables.size(); }

		void addRenderable(IRenderable* renderable) override;
		IRenderable* alterRenderable(unsigned int index) const override;
		const IRenderable* getRenderable(unsigned int index) const override;
		
		const glm::mat4& getModel() const override;
		Transform& alterModel() override { return model; };

		void draw(const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const override;
		//void drawButOne(unsigned int indexNOTDRAWN, const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const;
		//void drawOne(unsigned int index, const glm::mat4& transformProjectionView, const glm::vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const;
	};

}