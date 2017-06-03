#pragma once

#include "ILayer.h"
#include "Transform.h"
#include <algorithm>


namespace ginkgo {

	class IRenderable;
	class IPhongShader;
	class ICamera;
	class ICubeMap;

	class Layer : public ILayer
	{
	private:
		vector<IRenderable*> renderables;
		Transform model;
		static const unsigned int NO_TEXTURE = 0; //must be equal than 0 -> created textures will never have an id of 0

		static bool compareRenderables(IRenderable* r1, IRenderable* r2);
		static unsigned int determineTextureID(IRenderable* r1);
	public:
		Layer(const vector<IRenderable*>& renderables, const mat4& model = mat4());

		unsigned int getSize() const { return renderables.size(); }

		int addRenderable(IRenderable* renderable) override;
		IRenderable* alterRenderable(int UID) const override;
		const IRenderable* getRenderable(int UID) const override;
		void removeRenderable(int UID) override;
		
		const mat4& getModel() const override;
		ITransform& alterModel() override { return model; };

		void draw(const mat4& transformProjectionView, const vec3& cameraPosition, const IPhongShader& phongShader, const ICubeMap& cubeMap) const override;
		//void drawButOne(unsigned int indexNOTDRAWN, const mat4& transformProjectionView, const vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const;
		//void drawOne(unsigned int index, const mat4& transformProjectionView, const vec3& cameraPosition, const PhongShader& phongShader, const CubeMap& cubeMap) const;
	};

}