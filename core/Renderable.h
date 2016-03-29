#pragma once
#include "Entity.h"
#include "IRenderable.h"
//Should probably double check this

namespace ginkgo 
{
	class Renderable : public IRenderable
	{
	private:
		glm::vec3 scale;
		IRenderMesh const* mesh;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 velocity;
		glm::vec3 acceleration;

	public:
		Renderable(IRenderMesh const* mesh, const glm::vec3& pos, const glm::vec3& scl = glm::vec3(1, 1, 1), const glm::vec3& rot = glm::vec3(), const glm::vec3& vel = glm::vec3(), const glm::vec3& accel = glm::vec3());
		virtual const glm::vec3& getScale() const override;
		virtual void setScale(const glm::vec3& scl) override;
		virtual IRenderMesh const* getRenderMesh() const override;
		virtual void setRenderMesh(IRenderMesh const* mesh) override;
		virtual void render() override;

		virtual void tick(float elapsedTime) override;

		virtual const glm::vec3& getPosition() const override;
		virtual const glm::vec3& getVelocity() const override;
		virtual const glm::vec3& getAcceleration() const override;
		virtual const glm::vec3& getRotation() const override;

		virtual void setPosition(const glm::vec3& pos) override;
		virtual void setVelocity(const glm::vec3& vel) override;
		virtual void setAcceleration(const glm::vec3& acc) override;
		virtual void setRotation(const glm::vec3& ang) override;

		EntityType getEntityType() const override;
	};
}