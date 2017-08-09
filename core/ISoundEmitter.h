#pragma once

#include "IComponent.h"
#include <vector>

namespace ginkgo
{
	class ISoundEmitter : public IComponent
	{
	public:
		virtual void loadSound(const std::string name, const std::string filepath) = 0;

		virtual void emitSound(const std::string name, float volume) = 0;

		virtual void stopSounds() = 0;

		virtual void playMusic(const std::string name) = 0;

		virtual void pauseMusic() = 0;

		virtual void resumeMusic() = 0;
	};
	
	DECLSPEC_CORE ISoundEmitter* createSoundEmitter(IEntity* entity, bool isMusic);
}