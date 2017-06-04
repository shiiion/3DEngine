#pragma once
#include "Entity.h"
#include "IRenderComponent.h"


namespace ginkgo 
{
	class RenderComponent : public IRenderComponent
	{
	private:
		glm::vec3 scale;
		IRenderable* mesh;

		IEntity* parent;

	public:
		RenderComponent(IEntity* parent, IRenderable* mesh);
		const glm::vec3& getScale() const override;
		void setScale(const glm::vec3& scl) override;

		IEntity* getParent() override;

		void onTick(float elapsedTime) override;
		void onTickEnd(float elapsedTime) override;

		~RenderComponent() override;
	};
}