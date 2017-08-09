#pragma once

#include "ISoundEmitter.h"
#include <vector>

namespace ginkgo
{
	class SoundEmitter : public ISoundEmitter
	{
	public:
		void onTick(float elapsedTime);

		void onTickEnd(float elapsedTime);

		IEntity* getParent();

		SoundEmitter(IEntity* entity, bool isMusic);

		~SoundEmitter();

		void loadSound(const std::string name, const std::string filepath);

		void emitSound(const std::string name, float volume);

		void stopSounds();

		void playMusic(const std::string name) {}

		void pauseMusic();

		void resumeMusic();

		void onDetach() {}

	private:
		bool music;

		float soundVolume;

		std::vector<int> playingSounds;
		
		IEntity* parent;
	};
}