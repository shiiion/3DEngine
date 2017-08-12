#pragma once

#include <AL/alut.h>
#include "AudioBuffer.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>

using namespace glm;

namespace ginkgo
{
	class AudioManager
	{
	public:
		AudioManager();

		void addBuffer(const std::string name, const std::string filepath);

		int playSound(const std::string bufferName, bool loop, float volume, const vec3 &position, const vec3 &velocity, const quat &rotation);

		bool updateSound(int id, const vec3 &position, const vec3 &velocity, const quat &rotation);

		void pauseSound(int id);

		void resumeSound(int id);

		void eraseSound(int id);

		ALuint getSource(int id);

		void updateListener(const vec3 &position, const vec3 &velocity, const quat &rotation);

		void setListenerVolume(float volume);

		void setSourceVolume(int id, float volume);

		static AudioManager* getAudioManager();

	private:
		static AudioManager* instance;

		int soundCounter = 0;

		std::map<int, ALuint> sourceMap;

		std::map<std::string, AudioBuffer> bufferMap;
	};

}