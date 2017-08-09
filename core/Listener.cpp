#include "Listener.h"

namespace ginkgo
{ 
	void Listener::onTick(float elapsedTime) {}

	void Listener::onTickEnd(float elapsedTime)
	{
		AudioManager::getAudioManager()->updateListener(getParent()->getPosition(), getParent()->getVelocity(), getParent()->getRotation());
	}

	IEntity* Listener::getParent()
	{
		return parent;
	}

	Listener::Listener(IEntity* entity)
	{
		parent = entity;
	}

	Listener::~Listener()
	{
		//nuttin?
	}

	IComponent* createListener(IEntity* parent)
	{
		return new Listener(parent);
	}
}		