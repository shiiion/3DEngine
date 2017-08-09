#include "AudioManager.h"
#include "CoreReource.h"
#include <glm/gtc/quaternion.hpp>
#include <iostream>

using namespace glm;

namespace ginkgo
{
	AudioManager* AudioManager::instance = nullptr;

	AudioManager::AudioManager()
	{
		soundCounter = 0;

		alutInit(nullptr, nullptr);
	}

	void AudioManager::addBuffer(const std::string name, const std::string filepath)
	{
		AudioBuffer buffer;

		std::cout << alutGetError() << filepath.c_str() << "\n";

		buffer.sound = alutCreateBufferFromFile(filepath.c_str());

		std::cout << alutGetError() << "\n";

		alGetBufferi(buffer.sound, AL_CHANNELS, &buffer.channels);

		std::cout << buffer.channels;

		//can get other buffer properties here

		bufferMap.emplace(name, buffer);
	}

	int AudioManager::playSound(const std::string bufferName, bool loop, float volume, const vec3 &position, const vec3 &velocity, const quat &rotation)///source needs euler angles
	{
		ALuint buffer = bufferMap[bufferName].sound;

		ALuint source;

		alGenSources(1, &source);

		alSourcei(source, AL_LOOPING, loop);

		alSourcef(source, AL_GAIN, volume);

		alSource3f(source, AL_POSITION, position[0], position[1], position[2]);

		alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);

		vec3 orientation = glm::eulerAngles(rotation);

		alSource3f(source, AL_DIRECTION, orientation[0], orientation[1], orientation[2]);

		alSourcei(source, AL_BUFFER, buffer);

		alSourcePlay(source);

		int sourceState;

		alGetSourcei(source, AL_SOURCE_STATE, &sourceState);

		sourceMap[soundCounter] = source;

		return soundCounter++;
	}

	bool AudioManager::updateSound(int id, const vec3 &position, const vec3 &velocity, const quat &rotation)///source needs euler angles
	{
 		ALuint source = sourceMap[id];

		alSource3f(source, AL_POSITION, position[0], position[1], position[2]);

		alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
		
		vec3 orientation = glm::eulerAngles(rotation);

		alSource3f(source, AL_DIRECTION, orientation[0], orientation[1], orientation[2]);

		int sourceState;

		alGetSourcei(source, AL_SOURCE_STATE, &sourceState);

		return sourceState != AL_STOPPED;
	}

	void AudioManager::pauseSound(int id)
	{
		alSourcePause(sourceMap[id]);
	}

	void AudioManager::resumeSound(int id)
	{
		alSourcePlay(sourceMap[id]);
	}

	void AudioManager::eraseSound(int id)
	{
		sourceMap.erase(id);
	}

	ALuint AudioManager::getSource(int id)
	{
		return sourceMap[id];
	}

	void AudioManager::updateListener(const vec3 &position, const vec3 &velocity, const quat &rotation)
	{
		alListener3f(AL_POSITION, position[0], position[1], position[2]);

		alListener3f(AL_VELOCITY, velocity[0], velocity[1], velocity[2]);

		vec3 at = vec3(0, 0, -1) * rotation;

		vec3 up = vec3(0, 1, 0) * rotation;

		ALfloat directions[] = {at[0], at[1], at[2] , up[0], up[1], up[2]};

		alListenerfv(AL_ORIENTATION, directions);
	}

	void AudioManager::setListenerVolume(float volume)
	{
		alListenerf(AL_GAIN, volume);
	}

	void AudioManager::setSourceVolume(int id, float volume)
	{
		alSourcef(sourceMap[id], AL_GAIN, volume);
	}

	AudioManager* AudioManager::getAudioManager()
	{
		if (instance == nullptr)
		{
			instance = new AudioManager();
		} 
		
		return instance;
	}
}