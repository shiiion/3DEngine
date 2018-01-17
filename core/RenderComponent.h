#pragma once
#include "Entity.h"
#include "IRenderComponent.h"


namespace ginkgo 
{
	class RenderComponent : public IRenderComponent
	{
	private:
		IRenderable* mesh;

		IEntity* parent;

	public:
		RenderComponent(IEntity* parent, IRenderable* mesh);
		const vec3& getScale() const override;
		void setScale(const vec3& scl) override;
		const vec3& getPosition() const override;
		void setPosition(const vec3& pos) override;

		void setRotation(quat const& rotation) override;
		quat const& getRotation() const override;
		
		IEntity* getParent() override;

		void onTick(float elapsedTime) override;
		void onTickEnd(float elapsedTime) override;
		int type() const override;

		void onDetach() override;
	};
}